// Copyright 2019 Samolet LLC
// Author: kkozlov
// shared_params_manager_cmd.h

#ifndef SHARED_PARAMS_MANAGER_H_
# define SHARED_PARAMS_MANAGER_H_

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

namespace Samolet {

namespace Commands {
[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
public ref class SharedParamsManagerCmd : UI::IExternalCommand {
public:
  virtual UI::Result Execute(
      UI::ExternalCommandData ^commandData,
      String ^%message, DB::ElementSet ^elements);
};
}  // namespace Commands

namespace helper_classes {
ref class ParamManager {
public:
  ParamManager(AS::Application ^app, DB::Document ^doc);
  
  void	ExportParameters(String ^filename, DB::Document ^doc);
  int	ImportParameters(String ^filename, AS::Application ^app, DB::Document ^doc);
  void	RespondToEvent(Object ^s, EventArgs ^e);
                
private:
  AS::Application^	_app;
  DB::Document^		_doc;

  void    ImportFromFile(String ^filepath, DB::Document ^doc);
  void    ExportToFile(String ^filepath, DB::Document ^doc);
  void    ImportFromModel(DB::ModelPath ^modelpath, DB::Document ^activeDoc);
  
  DB::Document^	GetDocument(DB::ModelPath ^modelPath);
  void		ExportParameter(DB::DefinitionBindingMapIterator ^it,
                                IO::StreamWriter ^sw, DB::Document ^doc);
  void		ExportFamilyParameter(DB::Document ^doc,
                                      DB::SharedParameterElement ^spElem,
                                      IO::StreamWriter ^sw); 
  void		BindParameter(String ^line, DB::Definitions ^defs, DB::Document ^doc);
  void		BindFamilyParameter(DB::Document ^doc,String ^line, DB::Definitions ^def);
  
  static String^		GenerateHeader(void);
  static String^		CategoriesToString(DB::CategorySet ^set);
  static DB::CategorySet^	StringToCategories(String ^css, DB::Document ^doc);
  static CLG::ICollection<DB::Element ^>^	GetSharedParameterElements(DB::Document ^doc);  
};
}  // namespace helper_classes
}  // namespace Samolet

#endif  
