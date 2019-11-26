// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_by_unique_id_cmd.cpp

#include "select_by_unique_id_cmd.h"

#include "select_by_unique_id_form.h"

namespace Samolet {
namespace Commands {
UI::Result SelectByUniqueId::Execute(UI::ExternalCommandData ^cmdData,
                         String ^%msg, DB::ElementSet ^elements) {
  Gui::SelectByGuidForm^	form;

  form = gcnew Gui::SelectByGuidForm();
  form->ShowDialog();
  SelectElemsByUniqueId(cmdData->Application->ActiveUIDocument,
                        CheckUniqueIds(form->GetGuids()));
  return (UI::Result::Succeeded);
}

ICollectionElementId^ SelectByUniqueId::SelectElemsByUniqueId(
    UI::UIDocument^ uidoc, ICollectionString^ valid_ids) {
  LinkedListElementId^	elem_ids;
  IEnumerator^		guid_it;
  DB::Element^		elem;
  DB::Document^		doc;

  if (valid_ids == nullptr)
    return (nullptr);
  doc = uidoc->Document;
  elem_ids = gcnew LinkedListElementId();
  guid_it = valid_ids->GetEnumerator();
  while (guid_it->MoveNext()) {
    if ((elem = doc->GetElement(static_cast<String^>(
            guid_it->Current))) != nullptr)
      elem_ids->AddFirst(elem->Id);
  }
  uidoc->Selection->SetElementIds(elem_ids);
  return (nullptr);
}

ICollectionString^ SelectByUniqueId::CheckUniqueIds(array<String^>^ unique_ids) {
  using namespace System::Text::RegularExpressions;
  CLG::LinkedList<String^>^	valid_unique_id;
  IEnumerator^			it;
  String^			pattern;
  Regex^			rgx;
  String^			guid;

  if (unique_ids == nullptr)
    return (nullptr);
  pattern = "[A-Za-z0-9]{8}-[A-Za-z0-9]{4}"
      + "-[A-Za-z0-9]{4}-[A-Za-z0-9]{4}-[A-Za-z0-9]{12}";
  rgx = gcnew Regex(pattern);
  valid_unique_id = gcnew CLG::LinkedList<String^>();
  it = unique_ids->GetEnumerator();
  while (it->MoveNext()) {
    if (rgx->IsMatch((guid = static_cast<String^>(it->Current)))) {
      valid_unique_id->AddFirst(guid);
      LOG(String::Format("{0} is a valid guid", guid));
    }
  }
  return (valid_unique_id);
}

}  // namespace Commands
}  // namespace Samolet

