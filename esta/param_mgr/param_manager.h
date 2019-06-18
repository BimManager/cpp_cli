/*
 *    param_manager.h
 *    DefinitionFile
 *    |__DefinitionGroups
 *       |__DefinitionGroup
 *          |__Definitions
 *             |__Definition abstract
 *                |__ExternalDefinition
 *                |__InternalDefinition
 */

#ifndef ESTA_PARAM_MANAGER_H
# define ESTA_PARAM_MANAGER_H

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace CL = System::Collections;
namespace GCL = System::Collections::Generic;

namespace DB = Autodesk::Revit::DB;
namespace UI = Autodesk::Revit::UI;
namespace AS = Autodesk::Revit::ApplicationServices;

typedef System::Text::StringBuilder StrBuilder;

namespace Esta
{
    void PrintBinding(DB::Definition ^def, DB::ElementBinding ^binding);

    [Autodesk::Revit::Attributes::TransactionAttribute(
        Autodesk::Revit::Attributes::TransactionMode::Manual)]
    public ref class Command : UI::IExternalCommand
    {
    public:
        virtual UI::Result Execute(
            UI::ExternalCommandData ^commandData,
            System::String ^%message, 
            DB::ElementSet ^elements);
    }; /* Command */

    ref class ParamManager
    {
    public:
        /* ParamManager(AS::Application ^app, DB::UIDocument ^uidoc); */
        ParamManager(UI::UIDocument ^uidoc);
        StrBuilder  ^ParamManager::GetBindingGen(DB::ElementBinding ^binding);
        void GetInfoAboutEachDef(DB::DefinitionFile ^defFile);
        void IterateBindings(void (*pfn)(DB::Definition ^, DB::ElementBinding ^));
    private:
        UI::UIDocument  ^_uidoc;

        
        //DB::DefinitionFile  ^GetDefFile(AS::Application ^app, System::String ^spFilename);
        //GCL::List<DB::ExternalDefinition ^>   ^GetDefs(DB::DefinitionGroup ^defGrp);
    }; /* ParamManager */

    /* [Serializable]
    ref class ExtractedParameter
    {
    public:
        ExtractedParameter(DB::CategorySet ^set, DB::Definition ^def, char isType);
        
    private:
        DB::CategorySet ^_categories;
        DB::Definition  ^_definition;
        char            _isType;
    }*/
}


#endif