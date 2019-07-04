/*
 *  param_manager.cpp 
 */

#include "param_manager.h"
#include "form1.h"

namespace Esta
{
    UI::Result Command::Execute(UI::ExternalCommandData ^commandData,
        System::String ^%message, DB::ElementSet ^elements)
    {
        ParamManager        ^mgr;
        DB::Transaction     ^tr;
        Gui::FilePicker     ^dlg;
     
        mgr = gcnew ParamManager(
            commandData->Application->Application,
            commandData->Application->ActiveUIDocument);

        dlg = gcnew Gui::FilePicker();
        dlg->DialogDismissed +=
            gcnew System::EventHandler(
                mgr, &ParamManager::RespondToEvent);
        dlg->ShowDialog();
        return (UI::Result::Succeeded);
    }

    ParamManager::ParamManager(AS::Application ^app, UI::UIDocument ^uidoc)
    {   
        this->_app = app;
        this->_uidoc = uidoc;
        this->_doc = uidoc->Document;
    }

    String ^ParamManager::GenerateHeader(void)
    {
        return (String::Format("# This is a Revit shared parameter file.\n"
            "# Do not edit manually.\n*META\tVERSION\tMINVERSION\n"
            "META\t2\t1\n*GROUP\tID\tNAME\n"
            "GROUP\t1\tExported Parameters\n"
            "*PARAM\tGUID\tNAME\tDATATYPE\tDATACATEGORY\tGROUP\tVISIBLE\t"
            "DESCRIPTION\tUSERMODIFIABLE\tPARAMETERGROUP\tKIND\tCATEGORIES\t"
            "VARIESACROSSGROUPS"));
    }

    /* Exports only a shared parameter */
    void    ParamManager::ExportParameter(
                DB::DefinitionBindingMapIterator ^it,
                IO::StreamWriter ^sw)
    {
        DB::InternalDefinition              ^def;
        DB::ElementBinding                  ^binding;
        DB::SharedParameterElement          ^param;

        def = (DB::InternalDefinition ^)it->Key;
        binding = (DB::ElementBinding ^)it->Current;
        if (((DB::InternalDefinition ^)def)->BuiltInParameter
                    != DB::BuiltInParameter::INVALID)
            return ;
        param = RETURN_SP_ELEM(this->_uidoc->Document, def);
        if (param == nullptr)
            return ;
        sw->WriteLine(System::String::Format(FORMAT,
            param->GuidValue.ToString(), def->Name, def->ParameterType,
            String::Empty, PARAM_GROUP_NUMBER, 
            def->Visible ? "1" : "0", PARAM_DESCRIPTION_TXT, "1", 
            def->ParameterGroup,
            dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? 
            PARAM_TYPE : PARAM_INSTANCE,
            CategoriesToString(binding->Categories), 
            def->VariesAcrossGroups ? "1" : "0"));
    }

    void    ParamManager::ExportParameters(System::String ^filename)
    {
        IO::StreamWriter                    ^sw;
        CL::IEnumerator                     ^it;

        sw = gcnew IO::StreamWriter(filename, true, System::Text::Encoding::Unicode);
        sw->WriteLine(ParamManager::GenerateHeader());
        if (!this->_doc->IsFamilyDocument)
        {
            it = this->_uidoc->Document->ParameterBindings->ForwardIterator();
            while (it->MoveNext())
                this->ExportParameter((DB::DefinitionBindingMapIterator ^)it, sw);
        }
        else
        {
            it = ParamManager::GetSharedParameterElements(this->_doc)->GetEnumerator();
            while (it->MoveNext())
                this->ExportFamilyParameter(
                    (DB::SharedParameterElement ^)it->Current, sw);
        }
        sw->Close();
    }

    void    ParamManager::ImportParameters(String ^filepath)
    {
        IO::StreamReader    ^sr;
        String              ^line;
        DB::Definitions     ^defs;
        String              ^spfile;
        char                isfamdoc;

        spfile = this->_app->SharedParametersFilename;
        this->_app->SharedParametersFilename = filepath;
        defs = _app->OpenSharedParameterFile()->Groups
                   ->Item[PARAM_GROUP_NAME]->Definitions;
        sr = gcnew IO::StreamReader(filepath, System::Text::Encoding::Unicode);
        isfamdoc = this->_doc->IsFamilyDocument ? 1 : 0;
        while ((line = sr->ReadLine()) != nullptr)
        {
            if (line->StartsWith("PARAM"))
            {
                if (isfamdoc)
                    this->BindFamilyParameter(line, defs);
                else                    
                    this->BindParameter(line, defs);
            }
        }
        this->_app->SharedParametersFilename = spfile;
    }

    DB::CategorySet ^ParamManager::StringToCategories(System::String ^css, DB::Document ^doc)
    {
        array<System::String ^> ^cats;
        DB::Category            ^cat;
        DB::CategorySet         ^set;
        size_t                  i;
        DB::BuiltInCategory     ecat;

        cats = css->Split(';');
        set = gcnew DB::CategorySet();
        i = -1;
        while (++i < cats->Length)
        {
            if (!TRY_PARSE_ENUM(cats[i], true, ecat))
                continue ;
            cat = doc->Settings->Categories->Item[ecat];
            if (cat->AllowsBoundParameters)                    
                set->Insert(cat);
        }
        return (set);
    }

    void    ParamManager::BindParameter(String ^line, DB::Definitions ^defs)
    {
        array<String ^>             ^tab;
        DB::ElementBinding          ^binding;
        DB::CategorySet             ^cats;
        DB::BuiltInParameterGroup   paramgrp;

        tab = line->Split('\t');
        cats = StringToCategories(tab[PARAM_CATEGORIES], this->_uidoc->Document);
        if (tab[PARAM_KIND] == "T")
            binding = gcnew DB::TypeBinding(cats);
        else            
            binding = gcnew DB::InstanceBinding(cats);
        if (TRY_PARSE_ENUM(tab[PARAM_PARAMETER_GROUP], true, paramgrp) && !cats->IsEmpty)
            this->_uidoc->Document->ParameterBindings
                ->Insert(defs->Item[tab[PARAM_NAME]], binding, paramgrp);
    }

    System::String  ^ParamManager::CategoriesToString(DB::CategorySet ^set)
    {
        StrBuilder              ^gen;
        DB::CategorySetIterator ^it;

        gen = gcnew StrBuilder();
        it = set->ForwardIterator();
        while (it->MoveNext())
            gen->AppendFormat("{0};", ((DB::BuiltInCategory)
                (((DB::Category ^)it->Current)->Id->IntegerValue)).ToString());
        if (gen->Length > 1)
           gen->Remove(gen->Length - 1, 1);
        return (gen->ToString());
    }

    void    ParamManager::RespondToEvent(System::Object ^s, System::EventArgs ^e)
    {
        EventData::DismissedDialogEventArgs ^args;
        DB::Transaction                     ^tr;

        args = (EventData::DismissedDialogEventArgs ^)e;
        if (args->GetAction() == ACTION_EXPORT)
        {
            this->ExportParameters(args->GetFilepath());
            UI::TaskDialog::Show("Status", "The export has been completed.");
        }
        else if (args->GetAction() == ACTION_IMPORT)
        {
            tr = gcnew DB::Transaction(this->_uidoc->Document);
            tr->Start("Binding shared parameters");
            this->ImportParameters(args->GetFilepath());
            if (tr->Commit() == DB::TransactionStatus::Committed)
                UI::TaskDialog::Show("Status", "The import has been completed.");
        }
    }

    /*/void    ParamManager::ExportFamilyParameter(
                DB::FamilyParameter ^param, IO::StreamWriter ^sw)
    {
        DB::InternalDefinition  ^def;

        if (!param->IsShared)
            return ;
        def = (DB::InternalDefinition ^)param->Definition;
        sw->WriteLine(String::Format(FORMAT,
            param->GUID.ToString(),
            def->Name, def->ParameterType, String::Empty,
            "1", def->Visible ? "1" : "0", String::Empty,
            param->UserModifiable ? "1" : "0",
            def->ParameterGroup,
            param->IsInstance ? "I" : "T",
            String::Empty, String::Empty));
    }*/

    GCL::ICollection<DB::Element ^> 
        ^ParamManager::GetSharedParameterElements(DB::Document ^doc)
    {
        DB::FilteredElementCollector    ^col;
        DB::ElementClassFilter          ^classFltr;

        col = gcnew DB::FilteredElementCollector(doc);
        classFltr = gcnew DB::ElementClassFilter(DB::SharedParameterElement::typeid);
        return (col->WherePasses(classFltr)->ToElements());
    }

    void    ParamManager::ExportFamilyParameter(
        DB::SharedParameterElement ^spElem, IO::StreamWriter ^sw)
    {
        DB::InternalDefinition  ^def;
        DB::FamilyParameter     ^fParam;

        def = spElem->GetDefinition();
        fParam = this->_doc->FamilyManager->Parameter[def];
        sw->WriteLine(String::Format(FORMAT,
            spElem->GuidValue,
            def->Name, def->ParameterType, String::Empty,
            PARAM_GROUP_NUMBER, def->Visible ? "1" : "0", String::Empty,
            fParam != nullptr ? (fParam->UserModifiable ? "1" : "0") : PARAM_INVALID,
            def->ParameterGroup,
            fParam != nullptr ? (fParam->IsInstance ?
                PARAM_INSTANCE : PARAM_TYPE) : PARAM_INVALID,
            String::Empty, String::Empty));
    }

    void    ParamManager::BindFamilyParameter(String ^line, DB::Definitions ^defs)
    {
        array<String ^>             ^tab;
        DB::FamilyManager           ^fm;
        DB::BuiltInParameterGroup   pg;

       fm = this->_doc->FamilyManager;
       tab = line->Split('\t');
       if (System::Enum::TryParse(tab[PARAM_PARAMETER_GROUP], pg) 
            && tab[PARAM_KIND] != "-1")
            fm->AddParameter(
               (DB::ExternalDefinition ^)defs->Item[tab[PARAM_NAME]],
               pg,
               tab[PARAM_KIND] == "I" ? 1 : 0);
    }
}   

    /*DB::DefinitionFile ^CreateTempDefFile(AS::Application ^app)
    {
        IO::FileStream      ^fs;
        System::String      ^filename;

        filename = System::IO::Path::GetTempPath() + 
            System::Guid::NewGuid() + ".txt";
        fs = IO::File::Create(filename);
        fs->Close();
        app->SharedParametersFilename = filename;
        return (app->OpenSharedParameterFile());
    }*/