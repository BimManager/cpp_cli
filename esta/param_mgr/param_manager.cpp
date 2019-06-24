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
        UI::UIDocument      ^uidoc;
        AS::Application     ^app;
        ParamManager        ^mgr;
        DB::Transaction     ^tr;
        Gui::FilePicker     ^dlg;
     
        dlg = gcnew Gui::FilePicker();
        dlg->ShowDialog();
        app = commandData->Application->Application;
        uidoc = commandData->Application->ActiveUIDocument;    
        mgr = gcnew ParamManager(app, uidoc);
        if (!IO::File::Exists(FILENAME))                
            mgr->WriteToFile(FILENAME);
        else                
        {
            tr = gcnew DB::Transaction(uidoc->Document);
            tr->Start("Bind Parameters");
            mgr->ReadFile(FILENAME);
            tr->Commit();
        }
        UI::TaskDialog::Show("Gen", "The task has been completed.");
        return (UI::Result::Succeeded);
    }

    ParamManager::ParamManager(AS::Application ^app, UI::UIDocument ^uidoc)
    {   
        this->_app = app;
        this->_uidoc = uidoc;
    }

    void ParamManager::WriteToFile(System::String ^filename)
    {
        IO::StreamWriter                    ^sw;
        DB::DefinitionBindingMapIterator    ^it;
        DB::InternalDefinition              ^def;
        DB::ElementBinding                  ^binding;
        DB::SharedParameterElement          ^param;

        it = this->_uidoc->Document->ParameterBindings->ForwardIterator();
        sw = gcnew IO::StreamWriter(filename, true, System::Text::Encoding::UTF8);
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
            sw->WriteLine(System::String::Format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
            def->Name, def->ParameterGroup, def->ParameterType,
            CategoriesToStrings(binding->Categories),
            dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? "Type" : "Instance",
            param->GuidValue.ToString()));
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

    void ParamManager::ReadFile(System::String ^filename)
    {
        IO::StreamReader    ^sr;
        System::String      ^line;
        DB::DefinitionFile  ^defFile;
        DB::DefinitionGroup ^defGroup;

        defFile = CreateTempDefFile(this->_app);
        defGroup = defFile->Groups->Create(GROUP_NAME);
        sr = gcnew IO::StreamReader(
            IO::File::Open(filename, IO::FileMode::Open),
            System::Text::Encoding::UTF8);
        while ((line = sr->ReadLine()) != nullptr)
        {
            this->ProcessLine(line, defGroup);
        }
        sr->Close();
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

    void    ParamManager::ProcessLine(System::String ^line, DB::DefinitionGroup ^defGroup)
    {
        array<System::String ^>                 ^fields;
        DB::ExternalDefinitionCreationOptions   ^opts;
        DB::ExternalDefinition                  ^def;
        DB::ElementBinding                      ^binding;
        DB::CategorySet                         ^cats;
                
        fields = line->Split('\t');
        opts = gcnew DB::ExternalDefinitionCreationOptions(fields[PARAM_NAME], 
                STR_TO_ENUM(fields[PARAM_PARAMETER_TYPE], DB::ParameterType));
        opts->GUID = System::Guid(fields[PARAM_GUID]);
        opts->Visible = true;
        opts->UserModifiable = true;
        opts->Description = "N/A";
        def = (DB::ExternalDefinition ^)defGroup->Definitions->Create(opts);
        cats = StringsToCategories(fields[PARAM_CATEGORIES], this->_uidoc->Document);
        if (fields[PARAM_KIND] == PARAM_TYPE)
            binding = gcnew DB::TypeBinding(cats);
        else                
            binding = gcnew DB::InstanceBinding(cats);
        this->_uidoc->Document->ParameterBindings->Insert(def, binding, 
            STR_TO_ENUM(fields[PARAM_GROUP_TYPE],DB::BuiltInParameterGroup));
    }

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
}