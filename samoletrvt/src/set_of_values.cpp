// Copyright 2019 Samolet LLC
// Author: kkozlov
// set_of_values.cpp

#include "set_of_values.h"

#include "utils.h"

namespace Samolet {
namespace Helpers {
void SetOfValues::Initialize(String^ param_name) {
  _paramName = param_name;
  _defaultValue = String::Empty;
  _possibleValues = gcnew CLG::LinkedList<SelectableValue^>();
  _rvtParams = gcnew CLG::LinkedList<DB::Parameter^>();
  _can_vary_by_group = true;
  String^	param_names;

  if (_cannot_vary_by_group == nullptr)
    FillCannotChangeByGroup();
  if (_cannot_vary_by_group->ContainsKey(param_name))
    _can_vary_by_group = false;
}

void SetOfValues::FillCannotChangeByGroup(void) {
  String^		values_to_parse;
  array<String^>^	param_names;
  int			i;

  if (!Utils::ConfigFileManager::GetValueFromConfigFile(
          "cannotVaryByGroup", values_to_parse)) {
    param_names = values_to_parse->Split(';');
    _cannot_vary_by_group = gcnew DictionaryStringString(
        param_names->Length);
    i = -1;
    while (++i < param_names->Length)
      _cannot_vary_by_group->Add(param_names[i], nullptr);
  }
}

SetOfValues::SetOfValues(String^ param_name) {
  Initialize(param_name);
}

SetOfValues::SetOfValues(String ^param_name, array<String^> ^values) {
  int	i;

  Initialize(param_name);
  i =  values->Length;
  while (i--)
    _possibleValues->AddLast(gcnew SelectableValue(values[i]));
}

int	SetOfValues::CompareTo(Object ^obj) {
  SetOfValues	^other;

  if ((other = dynamic_cast<SetOfValues^>(obj)) != nullptr)
    return (String::Compare(_paramName, other->GetParamName()));
  return (-1);
}

String	^SetOfValues::GetParamName(void) {
  return (_paramName);
}

String	^SetOfValues::GetDefaultValue(void) {
  return (_defaultValue);
}

SelectableValue	^SetOfValues::GetValue(String ^valueName) {
  CL::IEnumerator	^it;

  it = _possibleValues->GetEnumerator();
  while (it->MoveNext())
    if (String::Compare(
            valueName,
            static_cast<SelectableValue^>
            (it->Current)->ToString()) == 0)
      return (static_cast<SelectableValue^>
              (it->Current));
  return (nullptr);            
}

array<String^>^ SetOfValues::GetValuesAsString(void) {
  array<String^>			^ret;
  CL::IEnumerator			^it;
  int					i;

  ret = gcnew array<String^>(_possibleValues->Count);
  it = _possibleValues->GetEnumerator();
  i = 0;
  while (it->MoveNext())
    ret[i++] = (static_cast<SelectableValue^>(
        it->Current))->ToString();
  Array::Sort(ret);
  return (ret);
}

CLG::LinkedList<DB::Parameter^>^ SetOfValues::GetRvtParams(void) {
  return (_rvtParams);
}

bool SetOfValues::CanVaryByGroup(void) {
  return (_can_vary_by_group);
}

void SetOfValues::set_rvt_elems(CLG::LinkedList<DB::Element^>^ elems) {
  _rvt_elems = elems;
}

LinkedListElement^ SetOfValues::rvt_elems(void) {
  return (_rvt_elems);
}

void SetOfValues::AddRvtParam(DB::Parameter ^rvtParam) {
  String	^extValue;

  if (_type_params == nullptr)
    _type_params = gcnew CL::Hashtable(
      _rvt_elems->Count, gcnew ParameterEqualityComparer());
  if ((extValue = Utils::RvtParamManager
       ::GetValueAsString(rvtParam)) != nullptr) {
    if (String::Compare(_defaultValue, String::Empty) == 0)
      _defaultValue = extValue;
    else if (String::Compare(_defaultValue, extValue))
      _defaultValue = "*";
  }
  if (_type_params->Contains(rvtParam))
    return;
  _type_params->Add(rvtParam, nullptr);
  _rvtParams->AddFirst(rvtParam);
}
void SetOfValues::AddRvtElem(DB::Element^ elem) {
  _rvt_elems->AddFirst(elem);
}

void SetOfValues::AddValue(SelectableValue ^val) {
  _possibleValues->AddLast(val);
}

void SetOfValues::SetSelectedElemsCount(int nElems) {
  _selectedElemsCount = nElems;
}

int SetOfValues::GetSelectedElemsCount(void) {
  return (_selectedElemsCount);
}
}  // namespace Helpers
}  // namespace Samolet
