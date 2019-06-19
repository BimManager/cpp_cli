/*
 *  param_manager.cpp 
 */

#include "param_manager.h"

namespace Esta
{
    UI::Result Command::Execute(
        UI::ExternalCommandData ^commandData,
        System::String ^%message, 
        DB::ElementSet ^elements)
        {
            ParamManager    ^mgr;
            DB::Transaction ^tr;
            UI::UIDocument    ^uidoc;

            uidoc = commandData->Application->ActiveUIDocument;    
            mgr = gcnew ParamManager(commandData->Application->Application, uidoc);
                
            if (!IO::File::Exists(FILENAME))                
                mgr->WriteToFile(FILENAME);
            else                
            {
                tr = gcnew DB::Transaction(uidoc->Document);
                tr->Start("Bind Parameters");
                mgr->ReadFile(FILENAME);
                tr->Commit();
            }
            UI::TaskDialog::Show("Gen", "The task has been done.");
            return (UI::Result::Succeeded);
        }


    ParamManager::ParamManager(AS::Application ^app, UI::UIDocument ^uidoc)
    {   
        this->_app = app;
        this->_uidoc = uidoc;
    }        

    void ParamManager::GetInfoAboutEachDef(DB::DefinitionFile ^defFile)
    {
        DB::DefinitionGroups    ^defGrps;
        CL::IEnumerator         ^itr;
        DB::DefinitionGroup     ^defGrp;
        DB::ExternalDefinition  ^extDef;

        defGrps = defFile->Groups;
        itr = defGrps->GetEnumerator();
        while (itr->MoveNext())
        {
            defGrp = (DB::DefinitionGroup ^)itr->Current;
            if (defGrp == nullptr)
                continue ;
        }
    }
    GCL::List<DB::ExternalDefinition ^>   ^getDefs(DB::DefinitionGroup ^defGrp)
    {
        GCL::List<DB::ExternalDefinition ^>   ^outDefs;
        DB::Definitions ^defs;
        CL::IEnumerator ^defItr;

        outDefs = gcnew GCL::List<DB::ExternalDefinition ^>();
        return (outDefs);
    }

    StrBuilder  ^ParamManager::GetBindingGen(DB::ElementBinding ^binding)
    {
        StrBuilder  ^outGen;
        DB::CategorySet ^cats;
        DB::CategorySetIterator ^it;

        outGen = gcnew StrBuilder();
        cats = binding->Categories;
        it = cats->ForwardIterator();
        while (it->MoveNext())
        {
            outGen->AppendLine(((DB::Category ^)it->Current)->Name);
        }
        (dynamic_cast<DB::TypeBinding ^>(binding)  != nullptr) 
            ? outGen->AppendLine("Type") : outGen->AppendLine("Instance");
        return (outGen);
    }

    void PrintBinding(DB::Definition ^def, DB::ElementBinding ^binding)
    {
        UI::TaskDialog::Show("Gen2",
            System::String::Format("Name: {0}\t Kind: {1}\t{2}\t{3}",
            def->Name,
            dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? "Type" : "Instance",
            def->ParameterGroup.ToString(),
            def->ParameterType.ToString()));
    }

    System::String ^GetUniqueId(System::String ^name, DB::Document ^doc)
    {
        DB::SharedParameterApplicableRule   ^rule;
        DB::ElementParameterFilter          ^filter;
        DB::FilteredElementCollector        ^col;
        DB::Element                         ^elem;

        rule = gcnew DB::SharedParameterApplicableRule(name);
        filter = gcnew DB::ElementParameterFilter(rule);
        col = gcnew DB::FilteredElementCollector(doc);
        elem = col->WherePasses(filter)->FirstElement();
        return (elem != nullptr ? elem->UniqueId : "NULL");
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
            param = dynamic_cast<DB::SharedParameterElement ^>
                    (this->_uidoc->Document->GetElement(def->Id));
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

    DB::DefinitionGroup ^ParamManager::CreateTempDefFile(AS::Application ^app)
    {
        IO::FileStream      ^fs;
        System::String      ^filename;
        DB::DefinitionFile  ^defFile;

        filename = System::IO::Path::GetTempPath() + 
            System::Guid::NewGuid() + ".txt";
        fs = IO::File::Create(filename);
        fs->Close();
        app->SharedParametersFilename = filename;
        defFile = app->OpenSharedParameterFile();
        return (defFile->Groups->Create(GROUP_NAME));
    }

    void ParamManager::ReadFile(System::String ^filename)
    {
        IO::StreamReader    ^sr;
        System::String      ^line;
        DB::Definition      ^def;
        DB::CategorySet     ^set;
        
        sr = gcnew IO::StreamReader(IO::File::Open(filename, IO::FileMode::Open),
                    System::Text::Encoding::UTF8);
        while ((line = sr->ReadLine()) != nullptr)
        {
            this->ProcessLine(line, this->CreateTempDefFile(this->_app));
        }
        sr->Close();
    }

    DB::BuiltInCategory StringToCategory(System::String ^cat)
    {
        return ((DB::BuiltInCategory)
            System::Enum::Parse(DB::BuiltInCategory::typeid, cat));
    }

    DB::CategorySet ^StringsToCategories(System::String ^css, DB::Document ^doc)
    {
        array<System::String ^> ^cats;
        DB::CategorySet         ^set;
        size_t                  i;

        cats = css->Split(',');
        set = gcnew DB::CategorySet();
        i = -1;
        while (++i < cats->Length)
        {
            set->Insert(doc->Settings->Categories->Item[StringToCategory(cats[i])]);
        }
        return (set);
    }

    DB::BuiltInParameterGroup StringToParameterGroup(System::String ^str)
    {
        return ((DB::BuiltInParameterGroup)
            System::Enum::Parse(DB::BuiltInParameterGroup::typeid, str));
    }

    void    ParamManager::ProcessLine(System::String ^line, DB::DefinitionGroup ^defGroup)
    {
        array<System::String ^> ^fields;
        DB::Definition          ^def;
        DB::ElementBinding      ^binding;

        fields = line->Split('\t');
        def = defGroup->Definitions->Create(gcnew DB::ExternalDefinitionCreationOptions(
                fields[PARAM_NAME], 
                (DB::ParameterType)System::Enum::Parse(DB::ParameterType::typeid,
                fields[PARAM_PARAMETER_TYPE])));             
        if (fields[PARAM_KIND] == PARAM_TYPE)
            binding = gcnew DB::TypeBinding(
                StringsToCategories(fields[PARAM_CATEGORIES], this->_uidoc->Document));
        else                
            binding = gcnew DB::InstanceBinding(
                StringsToCategories(fields[PARAM_CATEGORIES], this->_uidoc->Document));    
        this->BindParameter(def, binding, StringToParameterGroup(fields[PARAM_GROUP_TYPE]));
    }

    void    ParamManager::BindParameter(DB::Definition ^def,
                DB::ElementBinding ^binding, DB::BuiltInParameterGroup paramGroup)
    {
        this->_uidoc->Document->ParameterBindings->Insert(def, binding, paramGroup);
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

    /* void    ParamManager::IterateBindings(void (*pfn)(DB::Definition ^,DB::ElementBinding ^))
    {
        DB::BindingMap                      ^bndMap;
        DB::DefinitionBindingMapIterator    ^it;

        bndMap = this->_uidoc->Document->ParameterBindings;
        it = bndMap->ForwardIterator();
        while (it->MoveNext())
        {
            pfn(it->Key, (DB::ElementBinding ^)it->Current);
        }
    }*/

    /* CategorySet */
    /* InstanceBinding or TypeBinding */
    /* BindingMap */
    /* bindingMap.Insert(Definition ^, Binding ^) */

}

