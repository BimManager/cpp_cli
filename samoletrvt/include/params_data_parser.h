// Copyright 2019 Samolet LLC
// Author: kkozlov
// RvtDataParser.h

#ifndef PARAM_DATA_PARSER_H_
# define PARAM_DATA_PARSER_H_

# include "set_of_values.h"

namespace Samolet {
namespace DataManagement {  
ref class ParamDataParser {
 public:
  ParamDataParser(void);
  
  void ParseTxtFile(System::String ^filePath);
  void ParseWebData(System::String ^json);
  void SaveDataInDocument(Autodesk::Revit::DB::Document^ doc);
  array<Samolet::Helpers::SetOfValues^> ^GetValuesSets(void);
  array<System::String^>^ ParameterNames(void);
  
 private:
  void AddSetOfValues(Samolet::Helpers::SetOfValues ^set);
  void CarryOutDataParsing(void);
  
  CLG::LinkedList<Samolet::Helpers::SetOfValues^>^ setOfValues_;
};  
}  // namespace DataManagement
}  // namespace Samolet
#endif  // PARAM_DATA_PARSER_H_
