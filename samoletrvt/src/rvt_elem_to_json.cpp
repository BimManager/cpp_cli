// Copyright 2019 Samolet LLC
// Author: kkozlov
// rvt_elem_to_json.cpp

#include "rvt_elem_to_json.h"

#include "gui_utils.h"
#include "utils.h"
#include "internal_storage.h"
#include "web.h"
#include "validation.h"

namespace Samolet {
namespace Commands {
UI::Result ParamsToJsonCmd::Execute(
      UI::ExternalCommandData ^data,
      String ^%msg, DB::ElementSet ^elems) {
    return (CarryOutCmd(data, msg, elems));
}

UI::TaskDialog^	CreateCustomTaskDialog(void) {
  UI::TaskDialog^	dialog;

  dialog = gcnew UI::TaskDialog("Тип Экспорта");
  dialog->AddCommandLink(UI::TaskDialogCommandLinkId::CommandLink1,
                         "Сохранить в файл");
  dialog->AddCommandLink(UI::TaskDialogCommandLinkId::CommandLink2,
                         "Отправить на сервер");
  dialog->CommonButtons = UI::TaskDialogCommonButtons::Cancel;
  return (dialog);
}

UI::TaskDialog^	CreatePasswordTaskDialog(String^ instruction, String^ main_content) {
  UI::TaskDialog^	dialog;

  dialog = gcnew UI::TaskDialog("Пароль");
  dialog->MainInstruction = instruction;
  dialog->MainContent = main_content;
  dialog->MainIcon = UI::TaskDialogIcon::TaskDialogIconWarning;
  dialog->AddCommandLink(UI::TaskDialogCommandLinkId::CommandLink1,
                         "Введите пароль");
  dialog->CommonButtons = UI::TaskDialogCommonButtons::Cancel;
  return (dialog);
}

int	SaveJsonToFile(DB::Document^ doc, JavaScriptSerializer^ parser,
                       CLG::IEnumerator<DB::ElementId^>^ id_it) {
    String^		json;
    IO::StreamWriter^	sw;
    
    if ((json = Samolet::Gui::GuiUtils::SaveFile(
            "Json Files(*.json)|*.json",
            doc->Title)) != nullptr) {
      sw = gcnew IO::StreamWriter(json);
      while (id_it->MoveNext()) {
        json = parser->Serialize(doc->GetElement
                                 (id_it->Current));
        sw->Write(json);
        sw->Write("\n");
      }
      sw->Close();
    }
    return (SUCCESS);
}

int	SendJsonToServer(DB::Document^ doc, JavaScriptSerializer^ parser,
                         CLG::IEnumerator<DB::ElementId^>^ id_it, String^ method) {
  String^	json;
  String^	url;
  DB::Element^	elem;

#ifdef DEBUG
  Debug::WriteLine(String::Format("[RVT {0}] method: {1}", __FUNCTION__, method));
#endif
  if (Utils::ConfigFileManager::GetValueFromConfigFile("postRvtJsonDataUrl", url)) {
    UI::TaskDialog::Show("Error", "The url does not exist in the config file.");
    return (UNSPECIFIED_ERROR);
  }
  while (id_it->MoveNext()) {
    elem = doc->GetElement(id_it->Current);
    json = parser->Serialize(elem);
    if (method == "PUT")
      url = String::Format("{0}{1}/", url, elem->UniqueId);
#ifdef DEBUG
      Debug::WriteLine(String::Format("[RVT {0}] url: {1}", __FUNCTION__, url));
#endif
    Web::ReqResManager::JsonRequest(url, method, json);
  }
  return (SUCCESS);
}

void	ProcessAllLinks(DB::Document^ doc) {
  ICollectionElement^		links;
  IEnumeratorElement^		it;
  DB::RevitLinkInstance^	link;
  DB::Document^			linked_doc;
  
  
  links = Utils::RvtFilters::FindRevitLinkInstances(doc);
#ifdef DEBUG
  Debug::WriteLine(String::Format("[RVT ProcessAllLinks] The number of revit links is {0}",
                                  links->Count));
#endif
  it = links->GetEnumerator();
  while (it->MoveNext()) {
    link = static_cast<DB::RevitLinkInstance^>(it->Current);
#ifdef DEBUG
    if ((linked_doc = link->GetLinkDocument()) == nullptr)
      continue;
    Debug::WriteLine(String::Format(
        "[RVT ProcessAllLinks] Link's title: {0}",
        link->GetLinkDocument()->Title));
    Debug::WriteLine(String::Format(
        "[RVT ProcessAllLinks] A number of "
        "quantifiable elements dug up are "
        "{0}", Utils::RvtFilters::FindAllQuantifiableElemIds(doc)->Count));
#endif
  }
}

int	ParamsToJsonCmd::HandleValidationFailure(DB::Document^ doc) {
  namespace gui = Samolet::Gui;
  
  UI::TaskDialog^	td;
  gui::PasswordForm^	passwd_form;
  bool			correct_password;
  bool			result;

  result = false;
  if (!(result = Samolet::Validation::CarryOutValidation(doc))) {
    td = CreatePasswordTaskDialog("Проверка не пройдена.",
                                  "Для выгрузки без проверки введите пароль.");
    passwd_form = gcnew gui::PasswordForm("sam2019");
    if ((td->Show() == UI::TaskDialogResult::CommandLink1)) {
      passwd_form->ShowDialog();
      result = passwd_form->IsCorrectPassword ? true : false;
    }
  }
  return (result);
}

UI::Result ParamsToJsonCmd::CarryOutCmd(
    UI::ExternalCommandData ^data,
    String ^%msg, DB::ElementSet ^elems) {
  JavaScriptSerializer			^parser;
  CLG::IEnumerator<DB::ElementId^>	^id_it;
  UI::UIDocument			^uidoc;
  CLG::ICollection<DB::ElementId^>	^selIds;
  UI::TaskDialog^ 			dialog;
  UI::TaskDialogResult			result;
  int					error;
  String^				method;

  uidoc = data->Application->ActiveUIDocument;
  if (!HandleValidationFailure(uidoc->Document))
    return (UI::Result::Failed);
  
  parser = gcnew JavaScriptSerializer();
  parser->RegisterConverters(gcnew array<JavaScriptConverter^>
     { gcnew RvtElemConverter(uidoc->Document) });
  selIds = uidoc->Selection->GetElementIds();
  if (selIds->Count) {
    id_it = uidoc->Selection->GetElementIds()
        ->GetEnumerator();
    method = "PUT";
  } else {
    id_it = Utils::RvtFilters::FindAllBOQElemIds(uidoc->Document)
        ->GetEnumerator();
    method = "POST";
  }
  dialog = CreateCustomTaskDialog();
  result = dialog->Show();
  if (result == UI::TaskDialogResult::CommandLink1)
    error = SaveJsonToFile(uidoc->Document, parser, id_it);
  else if (result == UI::TaskDialogResult::CommandLink2)
    error = SendJsonToServer(uidoc->Document, parser, id_it, method);
  if (!error) {
    UI::TaskDialog::Show("Export Status",
                         "The export has completed successfully.");
    return (UI::Result::Succeeded);
  }
  ProcessAllLinks(uidoc->Document);
  return (UI::Result::Cancelled);
}

RvtElemConverter::RvtElemConverter(DB::Document ^doc)
    :_doc{doc}, _timestamp{DateTime::UtcNow} {}

CLG::IEnumerable<Type^>^
RvtElemConverter::SupportedTypes::get(void) {
    return (gcnew array<Type^> { DB::Element::typeid });
}

void RvtElemConverter::SerializeRvtElem(
    DB::ElementId ^id, CL::ArrayList ^paramVals) {
  if (id == DB::ElementId::InvalidElementId)
    return ;
  using namespace internal_storage;
  CL::IEnumerator ^pIt;
  DB::Element ^e;
  ES::Schema^	schema;
  ES::Entity^ entity;
  CLG::IDictionary<String^,String^>^ name_id_dic;
  //  CLG::IDictionary<String^, Object^> ^layersDic;
  //  DB::HostObjAttributes ^hostAttrs;
  IDictionaryStringObject^	params_values;
  DB::FamilyInstance^	fam_inst;
  
  e = _doc->GetElement(id);
  if ((schema = ES::Schema::Lookup(
          Guid(EXTRA_PARAMS_GEN_SCHEMA_GUID))) != nullptr &&
      (entity = e->GetEntity(schema))->IsValid()) {
    name_id_dic = entity->Get<CLG::IDictionary<String^,String^>^>(
        EXTRA_PARAMS_GEN_DATA_FIELD);
  }
  if ((fam_inst = dynamic_cast<DB::FamilyInstance^>(e)) != nullptr
      && fam_inst->SuperComponent != nullptr) {
    params_values = gcnew DictionaryStringObject();
    params_values->Add("Host", fam_inst->SuperComponent->UniqueId);
    paramVals->Add(params_values);
  }
  pIt = e->Parameters->GetEnumerator();
  
  while (pIt->MoveNext()) {
    paramVals->Add(this->ConvertParam(
        dynamic_cast<DB::Parameter^>(pIt->Current), name_id_dic));
  }
  
  /*    if ((hostAttrs = dynamic_cast
       <DB::HostObjAttributes^>(e)) != nullptr)
    {
      layersDic = this->ConvertLayers(hostAttrs);
      if (layersDic != nullptr)
        paramVals->Add(layersDic);
        }*/
  SerializeRvtElem(e->GetTypeId(), paramVals);
}


CLG::IDictionary<String^, Object^>^ RvtElemConverter::Serialize(
    Object ^obj, JavaScriptSerializer ^serializer) {
  DB::Element ^e;
  CLG::IDictionary<String^, Object^> ^ret;
  CL::ArrayList ^paramVals;
  
  if ((e = dynamic_cast<DB::Element^>(obj)) != nullptr) {
      paramVals = gcnew CL::ArrayList();
      SerializeRvtElem(e->Id, paramVals);
      ret = gcnew CLG::Dictionary<String^, Object^>();
      ret->Add("file_name", _doc->Title);
      ret->Add("date", _timestamp.ToString("yyyy-MM-ddTHH:mm:ssZ"));
      ret->Add("name", e->Name);
      ret->Add("uniqueId", e->UniqueId);
      ret->Add("parameters", paramVals);
      ret->Add("materials", ConvertMaterials(e));
    }
  return (ret);
}  

Object^ RvtElemConverter::Deserialize(
    CLG::IDictionary<String^, Object^> ^dic,
    Type ^type, JavaScriptSerializer ^serializer) {
  return (nullptr);
}

CLG::IDictionary<String^, Object^>^RvtElemConverter::ConvertParam(
    DB::Parameter ^p, CLG::IDictionary<String^,String^>^ name_id_dic) {
    CLG::IDictionary<String^, Object^> ^paramDic;
    DB::InternalDefinition ^int_def;
    String^ val;

    int_def = static_cast<DB::InternalDefinition^>(p->Definition);
    paramDic = gcnew CLG::Dictionary<String^, Object^>();
    if (int_def->BuiltInParameter == DB::BuiltInParameter::INVALID)
      paramDic->Add("Name", int_def->Name);
    else
      paramDic->Add("Name", int_def->BuiltInParameter.ToString());
    if (name_id_dic != nullptr) {
      if (name_id_dic->ContainsKey(p->Definition->Name))
        val = String::Compare(name_id_dic[int_def->Name], "-1") ?
            name_id_dic[int_def->Name] : nullptr;
    }
    paramDic->Add("Value", val != nullptr ? static_cast<Object^>(Int32::Parse(val)) :
                  Utils::RvtParamManager::GetValueAsString(_doc, p));
    return (paramDic);
}

  /*  CLG::IDictionary<String^, Object^>^*/
array<Object^>^
RvtElemConverter::ConvertMaterials(DB::Element ^e) {
    CLG::IDictionary<String^, Object^> ^matDic;
    CLG::IDictionary<String^, Object^> ^paramDic;
    CLG::LinkedList<Object^> ^pv;
    CLG::LinkedList<Object^> ^mats;
    CL::IEnumerator ^idIt;
    CL::IEnumerator ^pIt;
    array<Object^> ^res;
    DB::ElementId ^matId;
    DB::Material ^mat;
    DB::Parameter ^p;

    idIt = e->GetMaterialIds(false)->GetEnumerator();
    mats = gcnew CLG::LinkedList<Object^>();
    while (idIt->MoveNext())
      {
	pv = gcnew CLG::LinkedList<Object^>();
	matDic = gcnew CLG::Dictionary<String^, Object^>();
	matId = static_cast<DB::ElementId^>(idIt->Current);
	mat = static_cast<DB::Material^>(_doc->GetElement(matId));
	matDic->Add("Name", mat->Name);
	matDic->Add("Guid", mat->UniqueId);
	matDic->Add("Area", DB::UnitUtils
		    ::ConvertFromInternalUnits
		    (e->GetMaterialArea(matId, false),
		     DB::DisplayUnitType::DUT_SQUARE_METERS));
	matDic->Add("Volume", DB::UnitUtils
		    ::ConvertFromInternalUnits
		    (e->GetMaterialVolume(matId),
		     DB::DisplayUnitType::DUT_CUBIC_METERS));
	pIt = mat->Parameters->GetEnumerator();
	while (pIt->MoveNext())
	  {
	    paramDic = this->ConvertParam
                (dynamic_cast<DB::Parameter^>(pIt->Current), nullptr);
	    pv->AddLast(paramDic);
	  }
	res = gcnew array<Object^>(pv->Count);
	pv->CopyTo(res, 0);
	matDic->Add("Parameters", res);
	mats->AddLast(matDic);
      }
    res = gcnew array<Object^>(mats->Count);
    mats->CopyTo(res, 0);
    /*    ret = gcnew CLG::Dictionary<String^, Object^>();
	  ret->Add("Materials", res);*/
    return (res);
}

  CLG::IDictionary<String^, Object^>^
  RvtElemConverter::ConvertLayers
  (DB::HostObjAttributes ^hoa)
  {
    CLG::IDictionary<String^, Object^>	^ret;
    CL::ArrayList			^layers;
    CLG::IDictionary<String^, Object^>	^layerProps;
    DB::CompoundStructure		^cs;
    DB::CompoundStructureLayer		^layer;
    CL::IEnumerator			^it;

    ret = gcnew CLG::Dictionary<String^, Object^>();
    cs = hoa->GetCompoundStructure();
    if (cs == nullptr)
      return (nullptr);
    it = cs->GetLayers()->GetEnumerator();
    layers = gcnew CL::ArrayList();
    while (it->MoveNext())
      {
	layer = dynamic_cast
	  <DB::CompoundStructureLayer^>(it->Current);
	layerProps = gcnew CLG::Dictionary<String^, Object^>();
	layerProps->Add("Material",
			dynamic_cast<DB::Material^>
			(_doc->GetElement(layer->MaterialId))
			->Name);
	layerProps->Add("Width",
			DB::UnitUtils
			::ConvertFromInternalUnits
			(layer->Width,
			 DB::DisplayUnitType::DUT_MILLIMETERS));
	layerProps->Add("Function",
			layer->Function.ToString());
	layers->Add(layerProps);
      }
    ret->Add("Layers", layers);
    return (ret);
  }
}
}
