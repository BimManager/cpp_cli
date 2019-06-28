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

    void    ParamManager::ExportParameter(DB::DefinitionBindingMapIterator ^it,
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
        sw->WriteLine(System::String::Format(
        "PARAM\t{0}\t{1}\t{2}\t{3}\t{4}\t{5}" + 
        "\t{6}\t{7}\t{8}\t{9}\t{10}\t{11}",
        param->GuidValue.ToString(), def->Name, def->ParameterType,
        String::Empty, "1", def->Visible ? "1" : "0", "N/A", "1", def->ParameterGroup,
        dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? "T" : "I",
        CategoriesToStrings(binding->Categories), 
        def->VariesAcrossGroups ? "1" : "0"));
    }

    void    ParamManager::ExportParameters(System::String ^filename)
    {
        IO::StreamWriter                    ^sw;
        DB::DefinitionBindingMapIterator    ^it;

        it = this->_uidoc->Document->ParameterBindings->ForwardIterator();
        sw = gcnew IO::StreamWriter(filename, true, System::Text::Encoding::Unicode);
        sw->WriteLine(ParamManager::GenerateHeader());
        while (it->MoveNext())
        {
            this->ExportParameter(it, sw);
        }
        sw->Close();
    }

    void    ParamManager::ImportParameters(String ^filepath)
    {
        IO::StreamReader    ^sr;
        String              ^line;
        DB::Definitions     ^defs;
        String              ^spfile;

        spfile = this->_app->SharedParametersFilename;
        this->_app->SharedParametersFilename = filepath;
        defs = _app->OpenSharedParameterFile()->Groups
                    ->Item[PARAM_GROUP_NAME]->Definitions;
        sr = gcnew IO::StreamReader(filepath, System::Text::Encoding::Unicode);
        while ((line = sr->ReadLine()) != nullptr)
        {
            if (line->StartsWith("PARAM"))
                       this->BindParameters(line, defs);
        }
        this->_app->SharedParametersFilename = spfile;
    }

    DB::CategorySet ^ParamManager::StringsToCategories(System::String ^css, DB::Document ^doc)
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

    void    ParamManager::BindParameters(String ^line, DB::Definitions ^defs)
    {
        array<String ^>             ^tab;
        DB::ElementBinding          ^binding;
        DB::CategorySet             ^cats;
        DB::BuiltInParameterGroup   paramgrp;

        tab = line->Split('\t');
        cats = StringsToCategories(tab[PARAM_CATEGORIES], this->_uidoc->Document);
        if (tab[PARAM_KIND] == "T")
            binding = gcnew DB::TypeBinding(cats);
        else            
            binding = gcnew DB::InstanceBinding(cats);
        if (TRY_PARSE_ENUM(tab[PARAM_PARAMETER_GROUP], true, paramgrp) && !cats->IsEmpty)
            this->_uidoc->Document->ParameterBindings
                ->Insert(defs->Item[tab[PARAM_NAME]], binding, paramgrp);
    }

    System::String  ^ParamManager::CategoriesToStrings(DB::CategorySet ^set)
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
            UI::TaskDialog::Show("Gen", "The export has been completed.");
        }
        else if (args->GetAction() == ACTION_IMPORT)
        {
            tr = gcnew DB::Transaction(this->_uidoc->Document);
            tr->Start("Binding shared parameters");
            this->ImportParameters(args->GetFilepath());
            tr->Commit();
            UI::TaskDialog::Show("Gen", "The import has been completed.");
        }
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