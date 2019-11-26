// Copyright 2019 Samolet LLC
// Author: kkozlov
// selectable_value.cpp

#include "selectable_value.h"

namespace Samolet {
namespace Helpers {
SelectableValue::SelectableValue(String ^value)
    :_value{value}, _id{INVALID_ID}, _guid{String::Empty} {}

SelectableValue::SelectableValue(String ^value,
                                 int id)
    :_value{value}, _id{id}, _guid{String::Empty} {}

SelectableValue::SelectableValue(String ^value,
                                 int id, String ^guid)
    : _value{value}, _id{id}, _guid{guid} {}

String^ SelectableValue::ToString(void) {
  return (_value);
}

int SelectableValue::CompareTo(Object ^other) {
  SelectableValue ^otherValue;

  if ((otherValue = dynamic_cast
       <SelectableValue^>(other)) != nullptr)
    return (String::Compare(
        _value,otherValue->ToString()));
  return (-1);
}

String^ SelectableValue::GetGuid(void) {
  return (_guid);
}

int SelectableValue::GetId(void){
  return (_id);
}
}  // namespace HelperClasses
}  // namespace Samolet
