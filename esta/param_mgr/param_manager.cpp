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
            mgr = gcnew ParamManager(commandData->Application->ActiveUIDocument);
            mgr->IterateBindings(PrintBinding);
            return (UI::Result::Succeeded);
        }


    ParamManager::ParamManager(UI::UIDocument ^uidoc)
    {   
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
        UI::TaskDialog::Show("Gen", def->Name + ((System::Int32)def->ParameterGroup).ToString()
            + def->ParameterType.ToString() + def->UnitType.ToString() + 
            dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? "Type" : "Instance");
    }

    void    ParamManager::IterateBindings(void (*pfn)(DB::Definition ^,DB::ElementBinding ^))
    {
        DB::BindingMap                      ^bndMap;
        DB::DefinitionBindingMapIterator    ^it;

        bndMap = this->_uidoc->Document->ParameterBindings;
        it = bndMap->ForwardIterator();
        while (it->MoveNext())
        {
            pfn((DB::Definition ^)it->Key, (DB::ElementBinding ^)it->Current);
        }
    }

    /* CategorySet */
    /* InstanceBinding or TypeBinding */
    /* BindingMap */
    /* bindingMap.Insert(Definition ^, Binding ^) */

}

