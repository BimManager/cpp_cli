// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_values_cmd.cpp

#include "select_values_cmd.h"

#include "internal_storage.h"

namespace Samolet {
namespace Commands {
namespace  HLP = Samolet::Helpers;

bool AvailableUponSelection::IsCommandAvailable(
    UI::UIApplication ^uiapp, DB::CategorySet ^selCats) {
  selCats = nullptr;
  if (uiapp->ActiveUIDocument->Selection->GetElementIds()->Count)
    return (true);
  return (false);
}            

UI::Result SelValuesCmd::Execute(UI::ExternalCommandData ^data, 
		      String ^%msg, DB::ElementSet ^elements) {    
  return (CarryOutCmd(data, msg, elements));
}

UI::Result SelValuesCmd::CarryOutCmd(UI::ExternalCommandData ^data, 
			  String ^%msg, DB::ElementSet ^elements) {
  using  Samolet::DataManagement::ParamDataParser;
  using HLP::SetOfValues;
  
  UI::UIDocument^	uidoc;
  Gui::SelectionForm^	form;
  array<SetOfValues^>^	sets;
  ParamDataParser^	parser;

  uidoc = data->Application->ActiveUIDocument;
  parser = gcnew ParamDataParser();
  sets = parser->GetValuesSets();
  ExtractParameters(uidoc, sets);    
  form = gcnew Gui::SelectionForm(sets);
  form->ShowDialog();
  SetValues(uidoc->Document, form->GetSelected(), sets);
  return (UI::Result::Succeeded);
}

LinkedListElement^ SelValuesCmd::DealWithMaterials(DB::Document^ doc) {
  using namespace HLP;
   Gui::SelectionForm^	form;
   SetOfValues^		set;
   array<String^>^	names;
   IEnumerator^		elem_it;
   int			i;
   String^		picked;
   LinkedListElement^	elems;

   elems = gcnew LinkedListElement(Utils::RvtFilters::FindElemsOfClass(
       doc, Autodesk::Revit::DB::Material::typeid));
   names = gcnew array<String^>(elems->Count);
   elem_it = elems->GetEnumerator();
   i = 0;
   while (elem_it->MoveNext()) 
     names[i++] = static_cast<DB::Element^>(elem_it->Current)->Name;
   set = gcnew SetOfValues("Материал", names);
   form = gcnew Gui::SelectionForm(gcnew array<SetOfValues^> { set }, true);
   form->ShowDialog();
   picked = static_cast<String^>(form->GetSelected()["Материал"]);
   LOG("Material selected: " + picked);
   if (picked == nullptr)
     return (gcnew LinkedListElement());
   elem_it->Reset();
   while (elem_it->MoveNext()) 
     if (!String::Compare(static_cast<DB::Element^>(elem_it->Current)->Name,
                          picked)) {
       elems = gcnew LinkedListElement();
       elems->AddFirst(static_cast<DB::Element^>(elem_it->Current));
       break ;
     }
   return (elems);
}

void SelValuesCmd::FillSetWithParameters(DB::Element^ elem, HLP::SetOfValues^ set) {
  DB::Parameter^	param;
  DB::ElementId^	type_id;
  HLP::GroupHandler^	group_handler;
  IEnumeratorElement^	elem_it;

  if ((param = elem->LookupParameter(set->GetParamName())) != nullptr
      && !param->IsReadOnly) {
    set->AddRvtParam(param);
    LOG("parameter added: " + param->Definition->Name);
    return ;
  }
  if ((type_id = elem->GetTypeId()) != DB::ElementId::InvalidElementId) {
    FillSetWithParameters(elem->Document->GetElement(type_id), set);
  }
}

void SelValuesCmd::FillSetWithParametersInGroups(
    DB::Element^ elem, HLP::SetOfValues^ set) {
  HLP::GroupHandler^	group_handler;
  IEnumeratorElement^	elem_it;
  
  if (set->CanVaryByGroup()) {
    group_handler = gcnew HLP::GroupHandler(elem->Document);
    elem_it = group_handler->Execute(elem)->GetEnumerator();
    while (elem_it->MoveNext())
      FillSetWithParameters(elem_it->Current, set);
  }
}                           
 
void SelValuesCmd::ExtractParameters(
    UI::UIDocument ^uidoc, array<HLP::SetOfValues^> ^sets) {
  LinkedListElement^	elems;
  CL::IEnumerator^      elem_it;
  CL::IEnumerator^      set_it;
  DB::Element^		elem;
  HLP::SetOfValues^	set;

  if (uidoc->Selection->GetElementIds()->Count) {
    elems = gcnew LinkedListElement();
    elem_it = uidoc->Selection->GetElementIds()->GetEnumerator();
    while (elem_it->MoveNext())
      elems->AddLast(uidoc->Document->GetElement(
          static_cast<DB::ElementId^>(elem_it->Current)));
  } else {
    elems = DealWithMaterials(uidoc->Document);
  }
  LOG("the number of elems: " + elems->Count);
  HLP::SetOfValues::SetSelectedElemsCount(elems->Count);
  HLP::SetOfValues::set_rvt_elems(elems);
  elem_it = elems->GetEnumerator();
  set_it = sets->GetEnumerator();
  while (elem_it->MoveNext()) {
    elem = static_cast<DB::Element^>(elem_it->Current);
    while (set_it->MoveNext()) {
      set = static_cast<HLP::SetOfValues^>(set_it->Current);
      FillSetWithParameters(elem, set);
      FillSetWithParametersInGroups(elem, set);
    }
    set_it->Reset();
  }
}

void	SelValuesCmd::SetValues(DB::Document ^doc, 
                                CL::Hashtable ^selected,
                                array<HLP::SetOfValues^> ^sets) {
  using HLP::SetOfValues;
  
  DB::Transaction ^tr;
  CL::IEnumerator ^setIt;
  CL::IEnumerator ^pIt;
  SetOfValues       ^set;
  String          ^val;
  CLG::IDictionary<String^,String^>^ name_id_dic;

  if (!selected->Count)
    return ;
  setIt = sets->GetEnumerator();
  tr = gcnew DB::Transaction(doc);
  name_id_dic = gcnew CLG::Dictionary<String^,String^>(sets->Length);
  tr->Start("Set values");
  while (setIt->MoveNext()) {
    set = static_cast<SetOfValues^>(setIt->Current);
    if (set->GetRvtParams()->Count &&
        (val = static_cast<String^>(selected[set->GetParamName()])) != nullptr) {
      name_id_dic->Add(set->GetParamName(),
                       set->GetValue(val)->GetId().ToString());
      pIt = set->GetRvtParams()->GetEnumerator();
      while (pIt->MoveNext())
        Utils::RvtParamManager::SetStringValue(
            static_cast<DB::Parameter^>(pIt->Current), val);
        }
    }
  SetEntities(name_id_dic, SetOfValues::rvt_elems());
  tr->Commit();
}

void	SelValuesCmd::SetEntities(
    CLG::IDictionary<String^,String^>^ name_id_dic,
    CLG::LinkedList<DB::Element^>^ rvt_elems) {
  using namespace internal_storage;
  ES::Schema^	schema;
  ES::Entity^	entity;
  CLG::IEnumerator<DB::Element^>^	it;

  if ((schema = ES::Schema::Lookup(
          EXTRA_PARAMS_GEN_SCHEMA_GUID)) != nullptr) {
    entity = gcnew ES::Entity(schema);
    entity->Set(EXTRA_PARAMS_GEN_DATA_FIELD, name_id_dic);
    it = rvt_elems->GetEnumerator();
    while (it->MoveNext())
      it->Current->SetEntity(entity);
  }
}
} // namespace Commands
} // namespace Samolet

/*LinkedListElement^ SelValuesCmd::HandleGroupedElems(
    IEnumeratorElementId^ id_it, DB::Document ^doc) {
  LinkedListElement^		ret;
  DB::Element^			elem;
  Helpers::GroupHandler^	group_handler;
  IEnumeratorElement^		elem_it;

  ret = gcnew CLG::LinkedList<DB::Element^>();
  group_handler = gcnew Helpers::GroupHandler(doc);
  while (id_it->MoveNext()) {
    elem = doc->GetElement(id_it->Current);
    ret->AddLast(elem);
    elem_it = group_handler->Execute(elem)->GetEnumerator();
    while (elem_it->MoveNext())
      ret->AddLast(elem_it->Current);
  }
  return (ret);
  }*/


/*void SelValuesCmd::ExtractTypeParameters(DB::Document ^doc,
                                         DB::Element ^e,
                                         CL::IEnumerator ^set_it) {
  DB::ElementId^		id;		
  DB::ElementType^		type;
  DB::Parameter^		p;
  Helpers::SetOfValues^	set;

  if ((id = e->GetTypeId()) == DB::ElementId::InvalidElementId)
    return ;
  type = static_cast<DB::ElementType^>(doc->GetElement(id));
  while (set_it->MoveNext()) {
    set = static_cast<Helpers::SetOfValues^>(set_it->Current);
    if ((p = type->LookupParameter(set->GetParamName())) != nullptr)
      set->AddRvtParam(p);      
  }
  set_it->Reset();
  }*/
