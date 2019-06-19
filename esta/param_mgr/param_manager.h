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

# define FILENAME "C:\\Users\\kkozlov\\out.txt"

# define GROUP_NAME "Exported"
# define PARAM_TYPE "Type"
# define PARAM_NAME 0
# define PARAM_GROUP_TYPE 1
# define PARAM_PARAMETER_TYPE 2
# define PARAM_CATEGORIES 3
# define PARAM_KIND 4

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace CL = System::Collections;
namespace GCL = System::Collections::Generic;
namespace IO = System::IO;

namespace DB = Autodesk::Revit::DB;
namespace UI = Autodesk::Revit::UI;
namespace AS = Autodesk::Revit::ApplicationServices;

typedef System::Text::StringBuilder StrBuilder;

namespace Esta
{
    void PrintBinding(DB::Definition ^def, DB::ElementBinding ^binding);
    System::String ^CategoriesToStrings(DB::CategorySet ^set);
    DB::CategorySet ^StringsToCategories(System::String ^css, DB::Document ^doc);
    System::String ^GetUniqueId(System::String ^name, DB::Document ^doc);

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
        ParamManager(AS::Application ^app, UI::UIDocument ^uidoc);

        void WriteToFile(System::String ^filename);
        StrBuilder  ^ParamManager::GetBindingGen(DB::ElementBinding ^binding);
        void GetInfoAboutEachDef(DB::DefinitionFile ^defFile);
        //void IterateBindings(void (*pfn)(DB::Definition ^, DB::ElementBinding ^));
        void ReadFile(System::String ^filename);
        DB::DefinitionGroup ^CreateTempDefFile(AS::Application ^app);
        
    private:
        UI::UIDocument  ^_uidoc;
        AS::Application ^_app;

        void    ProcessLine(System::String ^line, DB::DefinitionGroup ^defGroup);
        void    BindParameter(DB::Definition ^def,
                DB::ElementBinding ^binding, DB::BuiltInParameterGroup paramGroup);
    }; /* ParamManager */
}


#endif