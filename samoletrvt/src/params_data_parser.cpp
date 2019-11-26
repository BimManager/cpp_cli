// Copyright 2019 Samolet LLC
// Author: kkozlov
// params_data_parser.cpp

#include "params_data_parser.h"

#include "set_of_values.h"
#include "utils.h"
#include "internal_storage.h"
#include "web.h"

namespace Samolet {
namespace DataManagement {
using namespace Web;
using namespace Helpers;
ParamDataParser::ParamDataParser(void)
    : setOfValues_{gcnew CLG::LinkedList<SetOfValues^>()} {}

void	ParamDataParser::ParseTxtFile(String ^filePath) {
  IO::StreamReader	^sr;
  String		^line;
  array<String^>	^paramValues;
  array<String^>	^values;
  SetOfValues		^valSet;

  sr = gcnew IO::StreamReader(
      filePath, System::Text::Encoding::Unicode);
  while ((line = sr->ReadLine()))
  {
    if (!line->Length)
      continue ;
    paramValues = line->Split('\t');
    if (paramValues->Length != 2)
      continue;
    values = paramValues[1]->Split(';');
    valSet = gcnew SetOfValues(
        paramValues[0], values);
    this->AddSetOfValues(valSet);
  }
  sr->Close();
}

void	ParamDataParser::ParseWebData(String ^json) {
  SamoletResponse	^res;
  CL::IEnumerator	^it;
  CL::IEnumerator	^valIt;
  SamoletResult		^result;
  SetOfValues		^set;
  SelectableValue	^val;

  res = ReqResManager::ParseResponse(json);
  if (res == nullptr)
    return ;
  it = res->Results->GetEnumerator();
  while (it->MoveNext()) {
    result = static_cast<SamoletResult^>(it->Current);
    json = ReqResManager::GetRequest(result->ApiUrl, TOKEN);
    res = ReqResManager::ParseResponse(json);
    set = gcnew SetOfValues(result->RevitParameter);
    valIt = res->Results->GetEnumerator();
    while (valIt->MoveNext()) {
      result = static_cast<SamoletResult^>(valIt->Current);
      set->AddValue(gcnew SelectableValue(
          result->Name, result->Id));
    }
    this->AddSetOfValues(set);
  }
}

void	ParamDataParser::CarryOutDataParsing(void) {
  String^		json_txt;

  if ((json_txt = Web::ReqResManager::GetRequest(
          RVT_PARAMS_URL, TOKEN)) != nullptr)
    this->ParseWebData(json_txt);
  if (Utils::ConfigFileManager
      ::GetFilePath("BOQValuesPath", json_txt) == 0)  
    this->ParseTxtFile(json_txt);
}

array<SetOfValues^>	^ParamDataParser::GetValuesSets(void) {
  array<SetOfValues^>	^ret;

  CarryOutDataParsing();
  ret = gcnew array<SetOfValues^>(setOfValues_->Count);
  setOfValues_->CopyTo(ret, 0);
  return (ret);
}

void	ParamDataParser::AddSetOfValues(SetOfValues ^newSet) {
  CLG::LinkedListNode<SetOfValues^>	^node;
  int					cmp_res;

  if ((node = setOfValues_->First) == nullptr) {
    setOfValues_->AddFirst(newSet);
    return ;
  }
  cmp_res = static_cast<SetOfValues^>(node->Value)->CompareTo(newSet);
  if (cmp_res == 0)  // do not add a duplicate
    return ;
  if (cmp_res > 0) {
    setOfValues_->AddBefore(node, newSet);
    return ;
  }
  while ((node = node->Next)) {
    if ((cmp_res = static_cast<SetOfValues^>(
            node->Value)->CompareTo(newSet)) == 0)
      return ;
     if (cmp_res > 0) {
       setOfValues_->AddBefore(node, newSet);
       return ;
     } 
  }
  setOfValues_->AddLast(newSet);
}

array<String^>^	ParamDataParser::ParameterNames(void) {
  typedef CLG::IEnumerator<SetOfValues^> IEnumerator;
  
  array<String^>^	ret;
  IEnumerator^		it;
  int			i;

  if (setOfValues_->Count == 0)
    CarryOutDataParsing();
  ret = gcnew array<String^>(setOfValues_->Count);
  it = setOfValues_->GetEnumerator();
  i = 0;
  while (it->MoveNext())
    ret[i++] = it->Current->GetParamName();
  return (ret);
}

void	ParamDataParser::SaveDataInDocument(DB::Document ^doc) {
  ES::DataStorage^	ds;
  ES::Entity^		entity;

  if ((ds = Samolet::Utils::RvtFilters::FindDataStorage(
          doc, GLOBAL_DATA_STORAGE_STR_STR)) != nullptr) {
    //    entity = gcnew ES::Entity(
  }

}
} // namespace DataManagement
} // namespace Samolet
