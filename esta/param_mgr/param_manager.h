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

# define STR_TO_ENUM(str, enum) (enum)System::Enum::Parse(enum::typeid, str)
# define RETURN_SP_ELEM(doc, def) dynamic_cast<DB::SharedParameterElement ^>(doc->GetElement(def->Id));

# define FILENAME "C:\\Users\\kkozlov\\out.txt"

# define GROUP_NAME "Exported"
# define PARAM_TYPE "Type"
# define PARAM_NAME 0
# define PARAM_GROUP_TYPE 1
# define PARAM_PARAMETER_TYPE 2
# define PARAM_CATEGORIES 3
# define PARAM_KIND 4
# define PARAM_GUID 5

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace CL = System::Collections;
namespace GCL = System::Collections::Generic;
namespace IO = System::IO;

namespace DB = Autodesk::Revit::DB;
namespace UI = Autodesk::Revit::UI;
namespace AS = Autodesk::Revit::ApplicationServices;

typedef System::String String;
typedef System::Text::StringBuilder StrBuilder;

namespace Esta
{
    DB::DefinitionFile ^CreateTempDefFile(AS::Application ^app);
    String ^CategoriesToStrings(DB::CategorySet ^set);
    DB::CategorySet ^StringsToCategories(String ^css, DB::Document ^doc);


    [Autodesk::Revit::Attributes::TransactionAttribute(
        Autodesk::Revit::Attributes::TransactionMode::Manual)]
    public ref class Command : UI::IExternalCommand
    {
    public:
        virtual UI::Result Execute(
            UI::ExternalCommandData ^commandData,
            String ^%message, 
            DB::ElementSet ^elements);
    }; /* Command */

    ref class ParamManager
    {
    public:
        ParamManager(AS::Application ^app, UI::UIDocument ^uidoc);

        void WriteToFile(String ^filename);
        void ReadFile(String ^filename);
             
    private:
        UI::UIDocument  ^_uidoc;
        AS::Application ^_app;

        void    ProcessLine(String ^line, DB::DefinitionGroup ^defGroup);
        
    }; /* ParamManager */
}

#endif  