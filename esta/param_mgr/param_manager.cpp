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
        /* dlg->DialogDismissed += 
        dlg->ShowDialog();
        return (UI::Result::Succeeded);*/
        /* if (!IO::File::Exists(FILENAME))                
            mgr->WriteToFile(FILENAME);
        else                
        {
            tr = gcnew DB::Transaction(uidoc->Document);
            tr->Start("Bind Parameters");
            mgr->ReadFile(FILENAME);
            tr->Commit();
        }*/
        UI::TaskDialog::Show("Gen", "The job has been completed.");
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
            "DESCRIPTION\tUSERMODIFIABLE\tPARAMETERGROUP\tKIND\tCATEGORIES"));
    }

    void ParamManager::WriteToFile(System::String ^filename)
    {
        IO::StreamWriter                    ^sw;
        DB::DefinitionBindingMapIterator    ^it;
        DB::InternalDefinition              ^def;
        DB::ElementBinding                  ^binding;
        DB::SharedParameterElement          ^param;

        it = this->_uidoc->Document->ParameterBindings->ForwardIterator();
        sw = gcnew IO::StreamWriter(filename, true, System::Text::Encoding::Unicode);
        sw->WriteLine(ParamManager::GenerateHeader());
        while (it->MoveNext())
        {
            def = (DB::InternalDefinition ^)it->Key;
            binding = (DB::ElementBinding ^)it->Current;
            if (((DB::InternalDefinition ^)def)->BuiltInParameter
                        != DB::BuiltInParameter::INVALID)
                continue ;
            param = RETURN_SP_ELEM(this->_uidoc->Document, def);
            if (param == nullptr)                    
                continue;
            sw->WriteLine(System::String::Format("PARAM\t{0}\t{1}\t{2}\t{3}\t{4}\t{5}" + 
            "\t{6}\t{7}\t{8}\t{9}\t{10}",
            param->GuidValue.ToString(), def->Name, def->ParameterType,
            String::Empty, "1", "1", "N/A", "1", def->ParameterGroup,
            dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? "T" : "I",
            CategoriesToStrings(binding->Categories)));
            
        }
        sw->Close();
    }

    DB::DefinitionFile ^CreateTempDefFile(AS::Application ^app)
    {
        IO::FileStream      ^fs;
        System::String      ^filename;

        filename = System::IO::Path::GetTempPath() + 
            System::Guid::NewGuid() + ".txt";
        fs = IO::File::Create(filename);
        fs->Close();
        app->SharedParametersFilename = filename;
        return (app->OpenSharedParameterFile());
    }

    /* void ParamManager::ReadFile(System::String ^filename)
    {
        IO::StreamReader    ^sr;
        System::String      ^line;
        DB::DefinitionFile  ^defFile;
        DB::DefinitionGroup ^defGroup;

        defFile = CreateTempDefFile(this->_app);
        defGroup = defFile->Groups->Create(PARAM_GROUP_NAME);
        sr = gcnew IO::StreamReader(
            IO::File::Open(filename, IO::FileMode::Open),
            System::Text::Encoding::UTF8);
        while ((line = sr->ReadLine()) != nullptr)
        {
            if (line->StartsWith("PARAM"))
                this->ProcessLine(line, defGroup);
        }
        sr->Close();
    }*/

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

    DB::CategorySet ^StringsToCategories(System::String ^css, DB::Document ^doc)
    {
        array<System::String ^> ^cats;
        DB::Category            ^cat;
        DB::CategorySet         ^set;
        size_t                  i;

        cats = css->Split(',');
        set = gcnew DB::CategorySet();
        i = -1;
        while (++i < cats->Length)
        {
            cat = doc->Settings->Categories
                    ->Item[STR_TO_ENUM(cats[i], DB::BuiltInCategory)];
            if (cat->AllowsBoundParameters)                    
                set->Insert(cat);
        }
        return (set);
    }

    void    ParamManager::BindParameters(String ^line, DB::Definitions ^defs)
    {
        array<String ^>     ^tab;
        DB::ElementBinding  ^binding;
        DB::CategorySet     ^cats;

        tab = line->Split('\t');
        cats = StringsToCategories(tab[PARAM_CATEGORIES], this->_uidoc->Document);
        if (tab[PARAM_KIND] == "T")
            binding = gcnew DB::TypeBinding(cats);
        else            
            binding = gcnew DB::InstanceBinding(cats);
        this->_uidoc->Document->ParameterBindings
            ->Insert(defs->Item[tab[PARAM_NAME]], binding, STR_TO_ENUM(
                tab[PARAM_PARAMETER_GROUP], DB::BuiltInParameterGroup));
    }

    /* void    ParamManager::ProcessLine(System::String ^line, DB::DefinitionGroup ^defGroup)
    {
        array<System::String ^>                 ^fields;
        DB::ExternalDefinitionCreationOptions   ^opts;
        DB::ExternalDefinition                  ^def;
        DB::ElementBinding                      ^binding;
        DB::CategorySet                         ^cats;
                
        fields = line->Split('\t');
        if (fields->Length < 12)
            {
                UI::TaskDialog::Show("Error", String::Format(
                    "The number of fields is {0}", fields->Length));
                return ;
            }
        opts = gcnew DB::ExternalDefinitionCreationOptions(fields[PARAM_NAME], 
                STR_TO_ENUM(fields[PARAM_DATATYPE], DB::ParameterType));
        opts->GUID = System::Guid(fields[PARAM_GUID]);
        opts->Visible = fields[PARAM_VISIBLE] == "1" ? 1 : 0;
        opts->UserModifiable = fields[PARAM_USERMODIFIABLE] == "1" ? 1 : 0;
        opts->Description = fields[PARAM_DESCRIPTION];
        def = (DB::ExternalDefinition ^)defGroup->Definitions->Create(opts);
        cats = StringsToCategories(fields[PARAM_CATEGORIES], this->_uidoc->Document);
        if (fields[PARAM_KIND] == "T")
            binding = gcnew DB::TypeBinding(cats);
        else                
            binding = gcnew DB::InstanceBinding(cats);
        this->_uidoc->Document->ParameterBindings->Insert(def, binding, 
            STR_TO_ENUM(fields[PARAM_PARAMETER_GROUP],DB::BuiltInParameterGroup));
    }*/

    System::String ^CategoriesToStrings(DB::CategorySet ^set)
    {
        StrBuilder              ^gen;
        DB::CategorySetIterator ^it;

        gen = gcnew StrBuilder();
        it = set->ForwardIterator();
        while (it->MoveNext())
            gen->AppendFormat("{0},", ((DB::BuiltInCategory)
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
            this->WriteToFile(args->GetFilepath());
        else if (args->GetAction() == ACTION_IMPORT)
        {
            tr = gcnew DB::Transaction(this->_uidoc->Document);
            tr->Start("Binding shared parameters");
            //this->ReadFile(args->GetFilepath());
            this->ImportParameters(args->GetFilepath());
            tr->Commit();
        }
            
    }
}   