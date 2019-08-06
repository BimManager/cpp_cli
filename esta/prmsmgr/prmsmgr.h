/*
 *    prmsmgr.h
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
# define TRY_PARSE_ENUM(str, ignorecase, res) \
    System::Enum::TryParse((str), (ignorecase), (res))
# define RETURN_SP_ELEM(doc, def) \
    dynamic_cast<DB::SharedParameterElement ^>(doc->GetElement(def->Id));
# define IS_FAMILY_PARAM_INSTANCE(doc, param)  \
    (doc->FamilyManager->GetAssociatedFamilyParameter(param)->IsInstance)

# define PARAM_GUID             1
# define PARAM_NAME             2
# define PARAM_DATATYPE         3
# define PARAM_DATACATEGORY     4
# define PARAM_GROUP            5
# define PARAM_VISIBLE          6
# define PARAM_DESCRIPTION      7
# define PARAM_USERMODIFIABLE   8
# define PARAM_PARAMETER_GROUP  9
# define PARAM_KIND             10
# define PARAM_CATEGORIES       11
# define PARAM_VARIES           12

# define PARAM_INVALID          "-1"
# define PARAM_TYPE             "T"
# define PARAM_INSTANCE         "I"

# define PARAM_GROUP_NAME       "Exported Parameters"
# define PARAM_GROUP_NUMBER     "1"
# define PARAM_DESCRIPTION_TXT  "N/A"
# define FORMAT "PARAM\t{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\t{9}\t{10}\t{11}"

# define SUCCESS                0
# define OPENING_FILE_ERROR     1


/* # define GROUP_NAME "Exported"
# define PARAM_TYPE "Type"
# define PARAM_NAME 0
# define PARAM_GROUP_TYPE 1
# define PARAM_PARAMETER_TYPE 2
# define PARAM_CATEGORIES 3
# define PARAM_KIND 4
# define PARAM_GUID 5 */

# using <System.dll>
# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

using System::Diagnostics::Debug;

namespace CL = System::Collections;
namespace GCL = System::Collections::Generic;
namespace IO = System::IO;

namespace DB = Autodesk::Revit::DB;
namespace UI = Autodesk::Revit::UI;
namespace AS = Autodesk::Revit::ApplicationServices;

typedef System::String String;
typedef System::Text::StringBuilder StrBuilder;

using Autodesk::Revit::ApplicationServices::Application;
using Autodesk::Revit::DB::Document;
using Autodesk::Revit::DB::ModelPath;


namespace Esta
{
    /* DB::DefinitionFile  ^CreateTempDefFile(AS::Application ^app); */
    String  ^CreateTempFile(void);

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
        ParamManager(Application ^app, Document ^doc);

        void                    ExportParameters(String ^filename, Document ^doc);
        int                     ImportParameters(String ^filename, Application ^app, Document ^doc);
        void                    RespondToEvent(System::Object ^s, System::EventArgs ^e);
             
    private:
        Application ^_app;
        Document    ^_doc;

        void    ImportFromFile(String ^filepath, Document ^doc);
        void    ExportToFile(String ^filepath, Document ^doc);
        void    ImportFromModel(ModelPath ^modelpath, Document ^activeDoc);
        //void    ExportToModel(ModelPath ^modelpath, Document ^activeDoc);

        Document                ^GetDocument(ModelPath ^modelPath);
        void                    ExportParameter(DB::DefinitionBindingMapIterator ^it,
                                    IO::StreamWriter ^sw, Document ^doc);
        void                    ExportFamilyParameter(Document ^doc,
                                    DB::SharedParameterElement ^spElem,
                                    IO::StreamWriter ^sw); 
        void                    BindParameter(String ^line, DB::Definitions ^defs, Document ^doc);
        void                    BindFamilyParameter(Document ^doc,
                                    String ^line, DB::Definitions ^def);
        static String           ^GenerateHeader(void);
        static String           ^CategoriesToString(DB::CategorySet ^set);
        static DB::CategorySet  ^StringToCategories(String ^css, DB::Document ^doc);
        static GCL::ICollection<DB::Element ^> ^GetSharedParameterElements(DB::Document ^doc);
    }; /* ParamManager */
}

#endif  