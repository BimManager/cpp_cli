// Copyright 2019 Samolet LLC
// Author: kkozlov
// utils.h

#include "utils.h"

namespace Samolet {
namespace Utils {
namespace sys = System;
namespace io = System::IO;
namespace txt = System::Text;
IO::MemoryStream^ Utils::ResourceManager::GetMemoryStream(
    REF::Assembly ^asmb, String ^rsName, String ^imgName) {
  RS::ResourceManager^	rm;
  
  rm = gcnew RS::ResourceManager(rsName, asmb);
  return (static_cast<IO::MemoryStream ^>(
      rm->GetObject(imgName)));
}

IMG::BitmapImage^ ResourceManager::MemoryStreamToImage(
    IO::MemoryStream ^ms) {
    IMG::BitmapImage         ^icon;

    icon = gcnew IMG::BitmapImage();
    ms->Seek(0, IO::SeekOrigin::Begin);
    icon->BeginInit();
    icon->StreamSource = ms;
    icon->EndInit();
    ms->Close();
    return (icon);
}

int ConfigFileManager::GetValueFromConfigFile(
    String ^key, String ^%out) {
  CF::Configuration^   config;
  int                  ret;

  config = CF::ConfigurationManager::
      OpenExeConfiguration(REF::Assembly
          ::GetExecutingAssembly()->Location);
  if (config->HasFile) 
    if ((ret = config->AppSettings
         ->Settings[key] != nullptr ? 0 : 1) == 0) 
      out = config->AppSettings->Settings[key]->Value;
  else 
    ret = 2;
  return (ret);
}

System::String^	ConfigFileManager::GetConnectionStringByName(System::String^ name) {
  System::Configuration::ConnectionStringSettings^	constr;
  System::String^					ret;

  constr = System::Configuration::ConfigurationManager::ConnectionStrings[name];
  if (constr != nullptr) {
    ret = constr->ConnectionString;
  }
  return (ret);
}


int ConfigFileManager::GetFilePath(
    String ^key, String ^%outFilePath) {
  REF::Assembly   ^asmb;
  IO::FileInfo    ^fi;
    
  asmb = REF::Assembly::GetExecutingAssembly();
  if (Utils::ConfigFileManager::
      GetValueFromConfigFile(key, outFilePath)) {
#ifdef DEBUG
    Debug::WriteLine(String::Format(
        "Key {0} does not exist in the config file.", key));
#endif
    return (CFM_KEY_DOES_NOT_EXIST);
  }
#ifdef DEBUG
  Debug::WriteLine(String::Format(
      "outFilePath: {0}",
      outFilePath));
#endif
  fi = gcnew IO::FileInfo(outFilePath);
  if (!fi->Exists)
    return (CFM_FILE_DOES_NOT_EXIT);
  return (SUCCESS);
}

String^ FileUtils::CreateTempFile(void) {
  IO::FileStream  ^fs;
  String          ^filename;

  filename = System::IO::Path::GetTempPath() + 
      System::Guid::NewGuid() + ".txt";
  fs = IO::File::Create(filename);
  fs->Close();
#ifdef DEBUG
  Debug::WriteLine("Filename: " + filename);
#endif
  return (filename);
}

void FileUtils::SaveToFile(String ^filepath, String ^data) {
  io::Stream		^stream;
  array<unsigned char>	^bytes;
  txt::Encoding		^utf8;
  
  stream = io::File::Open(filepath, io::FileMode::Create);
  utf8 = txt::Encoding::UTF8;
  bytes = utf8->GetBytes(data);
  stream->Write(bytes, 0, utf8->GetByteCount(data));
  stream->Close();
}

array<String^>^ FileUtils::ReadNLines(String^ filepath, int n_lines) {
  io::StreamReader^	sr;
  sys::String^		line;
  array<sys::String^>^	lines;
  int			i;
  
  if (io::File::Exists(filepath) && n_lines) {
    lines = gcnew array<sys::String^>(n_lines);
    sr = gcnew io::StreamReader(filepath, txt::Encoding::UTF8);
    i = 0;
    while ((line = sr->ReadLine()) && n_lines--)
      lines[i] = line;
    sr->Close();
    return (lines);
  }
  return (nullptr);
}

array<System::String^>^
HandyUtils::StringifyEnumValues(Type ^enum_type) {
  Array^		enum_vals;			
  CL::IEnumerator^	it;
  array<String^>^	ret;
  int			i;
  
  enum_vals = Enum::GetValues(enum_type);
  ret = gcnew array<String^>(enum_vals->Length);
  it = enum_vals->GetEnumerator();
  i = 0;
  while (it->MoveNext())
    ret[i++] = it->Current->ToString();
  Array::Sort(ret);
  return (ret);

}

System::Security::SecureString^
HandyUtils::StringToSecureString(System::String^ string) {
  int					i;
  array<wchar_t>^			chars;
  System::Security::SecureString^	secure_string;

  chars = string->ToCharArray();
  secure_string = gcnew System::Security::SecureString();
  i = -1;
  while (++i < chars->Length)
    secure_string->AppendChar(chars[i]);
  secure_string->MakeReadOnly();
  return (secure_string);
}
System::String^ HandyUtils::SecureStringToString(
    System::Security::SecureString^ secure_string) {
  namespace interop = System::Runtime::InteropServices;
  IntPtr	int_ptr;
  wchar_t	*p_bstr;
  String^	string;	

  int_ptr = interop::Marshal::SecureStringToBSTR(secure_string);
  p_bstr = (wchar_t *)int_ptr.ToPointer();
  string = gcnew String(p_bstr);
  interop::Marshal::FreeBSTR(int_ptr);
  return (string);
}

void RvtParamManager::SetStringValue(
    DB::Parameter ^par, String ^valAsStr) {
  switch (par->StorageType) {
    case DB::StorageType::Integer:
      int valAsInt;
      if (Int32::TryParse(valAsStr, valAsInt))
        par->Set(valAsInt);
      else if (valAsStr->Length)
        par->Set(valAsStr->Length == 3 ? 1 : 0);
      break ;
    case DB::StorageType::Double:
      double valAsDbl;
      if (Double::TryParse(valAsStr, valAsDbl))
        par->Set(valAsDbl);
      break ;
    case DB::StorageType::String:
      par->Set(valAsStr);
      break ;
    case DB::StorageType::ElementId:
      int idAsInt;
      if (Int32::TryParse(valAsStr, idAsInt))
        par->Set(gcnew DB::ElementId(idAsInt));
      break ;
    case DB::StorageType::None:
      break ;
  }   
}

String^ RvtParamManager::GetValueAsString(DB::Parameter ^p) {
  switch (p->StorageType) {
    case DB::StorageType::Integer:
      return (p->AsInteger().ToString());
    case DB::StorageType::Double:
      return (p->AsValueString()->Split(' ')[0]);
    case DB::StorageType::String:
      return (p->AsString());
    case DB::StorageType::ElementId:
      return (p->AsElementId()
              ->IntegerValue
              .ToString());
    case DB::StorageType::None:
      return ("None");
    default:
      return ("N/A");
  }
}

String^ RvtParamManager::GetValueAsString(
    DB::Document ^doc, DB::Parameter ^p) {
  DB::ElementId^	id;
  
  switch (p->StorageType) {
    case DB::StorageType::Integer:
      return (p->HasValue ? p->AsInteger().ToString() : String::Empty);
    case DB::StorageType::Double:
      return (p->AsValueString()->Split(' ')[0]);
    case DB::StorageType::String:
      return (p->AsString() != nullptr ?
              p->AsString() : String::Empty);
    case DB::StorageType::ElementId:
        if ((id = p->AsElementId())
          == DB::ElementId::InvalidElementId)
        return (p->AsValueString());
      else if (static_cast<int>(DB::BuiltInParameter::HOST_ID_PARAM)
               == p->Id->IntegerValue)
        return (doc->GetElement(id)->UniqueId);
      else if (static_cast<int>(DB::BuiltInParameter::ELEM_CATEGORY_PARAM)
               == p->Id->IntegerValue)
        return (static_cast<DB::BuiltInCategory>(
            doc->Settings->Categories->Item[p->AsValueString()]
            ->Id->IntegerValue).ToString());
      return (RvtParamManager::GetElemName(doc->GetElement(id)));
    case DB::StorageType::None:
      return ("None");
    default:
      return ("N/A");
  }
}

String^ RvtParamManager::GetElemName(DB::Element ^e) {
  DB::Level ^lvl;
  DB::ElementType ^type;
  DB::Material ^mat;
  DB::HostObject ^host;
  String ^idStr;

  if (e == nullptr)
    return ("");
  idStr = e->Id->IntegerValue.ToString();
  if ((lvl = dynamic_cast<DB::Level^>(e)) != nullptr)
    return (String::Format("{0}", lvl->Parameter
                           [DB::BuiltInParameter::DATUM_TEXT]
                           ->AsString(), idStr));
  else if ((type = dynamic_cast<DB::ElementType^>(e)) != nullptr)
    return (String::Format("{0}", type->FamilyName, idStr));
  else if ((mat = dynamic_cast<DB::Material^>(e)) != nullptr)
    return (String::Format("{0}", mat->Parameter
	     [DB::BuiltInParameter::MATERIAL_NAME]
	     ->AsString(), idStr));
  else if ((host = dynamic_cast<DB::HostObject^>(e)) != nullptr)
    return (String::Format("{0}", host->Name, idStr));
  return ("N/A");
}

CLG::ICollection<DB::Element^>^ RvtFilters::FindElemsOfClassByTypeId(
    DB::Document ^doc, Type ^type, DB::ElementId^ type_id) {
  DB::FilteredElementCollector    ^col;
  DB::ElementParameterFilter      ^idFltr;

  idFltr = gcnew DB::ElementParameterFilter(
      DB::ParameterFilterRuleFactory::CreateEqualsRule(
          gcnew DB::ElementId(DB::BuiltInParameter
                              ::ELEM_TYPE_PARAM), type_id)); 
  col = gcnew DB::FilteredElementCollector(doc);
  return (col->OfClass(type)
          ->WherePasses(idFltr)
          ->WhereElementIsNotElementType()
          ->ToElements());
}

ES::DataStorage^ RvtFilters::FindDataStorage(DB::Document^ doc, String^ name) {
  DB::FilteredElementCollector^	col;
  CL::IEnumerator^		it;

  col = gcnew DB::FilteredElementCollector(doc);
  it = col->OfClass(ES::DataStorage::typeid)->ToElements()->GetEnumerator();
  while (it->MoveNext())
    if (!String::Compare(static_cast<DB::Element^>(it->Current)->Name, name))
      return (static_cast<ES::DataStorage^>(it->Current));
  return (nullptr);
}

ICollectionCategory^ RvtFilters::FindCategoriesByBuiltInName(
    DB::Document^ doc, CL::IEnumerable^ names) {
  IEnumerator^		name_it;
  LinkedListCategory^	categories;
  DB::Category^		cur_cat;
  DB::Categories^	cats_in_doc;
  DB::BuiltInCategory	built_in_cat;

  cats_in_doc = doc->Settings->Categories;
  name_it = names->GetEnumerator();
  categories = gcnew LinkedListCategory();
  while (name_it->MoveNext()) {
    if (Enum::TryParse(static_cast<String^>(name_it->Current),
                       true, built_in_cat)) {
      categories->AddLast(cats_in_doc->Item[built_in_cat]);
      LOG(built_in_cat.ToString());
    }
  }
  return (categories);
}

ICollectionCategory^ RvtFilters::FindAllQtyCategories(DB::Document ^doc) {
  CL::IEnumerator			^catIt;
  DB::Category				^cat;
  LinkedListCategory^	qtyCats;
  array<DB::Category^>			^ret;

  catIt = doc->Settings->Categories->GetEnumerator();
  qtyCats = gcnew LinkedListCategory();
  while (catIt->MoveNext()) {
    cat = static_cast<DB::Category^>(catIt->Current);
    if (cat->HasMaterialQuantities)
      qtyCats->AddLast(cat);
  }
  return (qtyCats);
}

ICollectionElementId^ RvtFilters::FindAllElementsByCategory(
    Autodesk::Revit::DB::Document^ doc, ICollectionCategory^ cats) {
  DB::FilteredElementCollector^		col;
  DB::LogicalOrFilter^			or_fltr;
  CLG::IList<DB::ElementFilter^>^	cat_fltrs;
  CLG::IEnumerator<DB::Category^>^	cat_it;
  
  cat_it = cats->GetEnumerator();
  cat_fltrs = gcnew CLG::List<DB::ElementFilter^>(cats->Count);
  while (cat_it->MoveNext())
    cat_fltrs->Add(gcnew DB::ElementCategoryFilter(cat_it->Current->Id));
  or_fltr = gcnew DB::LogicalOrFilter(cat_fltrs);
  col = gcnew DB::FilteredElementCollector(doc);
  return (col
          ->WherePasses(or_fltr)
          ->WhereElementIsNotElementType()
          ->ToElementIds());

}

ICollectionElementId^ RvtFilters::FindAllQuantifiableElemIds(
    DB::Document ^doc) {
  ICollectionCategory^			qty_cats;

  qty_cats = FindAllQtyCategories(doc);
  return (FindAllElementsByCategory(doc, qty_cats));
}

ICollectionElementId^ RvtFilters::FindAllBOQElemIds(DB::Document^ doc) {
  ICollectionCategory^	qty_cats;
  ICollectionCategory^	adjunct_cats;
  LinkedListCategory^	boq_cats;
  CLG::IEnumerator<DB::Category^>^	cat_it;

  qty_cats = FindAllQtyCategories(doc);
  adjunct_cats = FindCategoriesByBuiltInName(doc, gcnew array<String^> {
      "OST_PipeCurves", "OST_DuctCurves", "OST_CableTray", "OST_Rebar",
          "OST_PipeFitting", "OST_PipeInsulations", "OST_PipeAccessory",
          "OST_DuctFitting", "OST_DuctInsulations", "OST_DuctAccessory",
          "OST_DuctTerminal"
    });
  boq_cats = gcnew LinkedListCategory();
  cat_it = qty_cats->GetEnumerator();
  while (cat_it->MoveNext())
    boq_cats->AddLast(cat_it->Current);
  cat_it = adjunct_cats->GetEnumerator();
  while (cat_it->MoveNext())
    boq_cats->AddLast(cat_it->Current);
  return (FindAllElementsByCategory(doc, boq_cats));
}

ICollectionElement^ RvtFilters::FindElemsOfClass(DB::Document^ doc, Type^ type) {
  DB::FilteredElementCollector collector(doc);

  return (collector
          .OfClass(type)
          ->WhereElementIsNotElementType()
          ->ToElements());
}

ICollectionElement^ RvtFilters::FindRevitLinkInstances(DB::Document^ doc) {
  return (FindElemsOfClass(doc, DB::RevitLinkInstance::typeid));
}

void ViewsManager::ResetOverrideGraphicSettings(
    DB::Document ^doc, DB::ElementId ^eid) {
  DB::OverrideGraphicSettings	^ogs;

  ogs = gcnew DB::OverrideGraphicSettings();
  ogs->SetSurfaceForegroundPatternId(
      DB::ElementId::InvalidElementId);
  ogs->SetSurfaceForegroundPatternColor(
      DB::Color::InvalidColorValue);
  doc->ActiveView->SetElementOverrides(
      eid, ogs);
}

ICollectionElement^ RvtFilters::FindMEPCurvesIntersectingWall(
    DB::Document^ doc, DB::Wall^ wall) {
  DB::FilteredElementCollector		collector(doc);
  DB::Outline				outline(wall->BoundingBox[nullptr]->Min,
                                                wall->BoundingBox[nullptr]->Max);
  DB::BoundingBoxIntersectsFilter	outline_filter(%outline);

  return(collector.OfClass(DB::MEPCurve::typeid)
         ->WherePasses(%outline_filter)
         ->ToElements());
}

DB::FamilySymbol^ RvtFilters::FindFamilySymbolByName(
    DB::Document^ doc, String^ family_name, String^ symbol_name) {
  DB::FilteredElementCollector^	collector;
  DB::FamilySymbolFilter^	symbol_filter;
  DB::Element^			family;
  DB::Element^			family_symbol;
  IEnumeratorElement^		elem_it;
  
  collector = gcnew DB::FilteredElementCollector(doc);
  elem_it = collector
      ->OfClass(DB::Family::typeid)
      ->ToElements()->GetEnumerator();
  while (elem_it->MoveNext())
    if (!String::Compare(family_name, elem_it->Current->Name)) {
      family = elem_it->Current;
      LOG(elem_it->Current->Name);
    }  
  if (family != nullptr) {
    LOG("Family found: " + family->Name);
    symbol_filter = gcnew DB::FamilySymbolFilter(family->Id);
    LOG(family->Id->IntegerValue);
    delete collector;
    collector = gcnew DB::FilteredElementCollector(doc);
    elem_it = collector
        ->WherePasses(symbol_filter)
        ->ToElements()
        ->GetEnumerator();
    while (elem_it->MoveNext()) {
      if (!String::Compare(symbol_name, elem_it->Current->Name)) {
        family_symbol = elem_it->Current;
        LOG("FamilySymbol found: " + family_symbol->Name);
        return (static_cast<DB::FamilySymbol^>(family_symbol));
      }
    }
  } else {
    LOG("NULL");
  }
  return (nullptr);
}


DB::Solid^ RvtGeometryHandler::GetSolid(DB::GeometryInstance^ geom_inst) {
  IEnumeratorGeometryObject^	geom_obj_it;
  DB::Solid^			solid;
  DB::GeometryInstance^		nested_geom_inst;

  geom_obj_it = geom_inst->SymbolGeometry->GetEnumerator();
  while (geom_obj_it->MoveNext()) {
    if ((solid = dynamic_cast<DB::Solid^>(
            geom_obj_it->Current)) != nullptr && solid->Volume)
      return (solid);
    else if ((nested_geom_inst = dynamic_cast<DB::GeometryInstance^>(
        geom_obj_it->Current)) != nullptr)
      return (GetSolid(nested_geom_inst));
  }
  return (nullptr);
}

DB::Solid^ RvtGeometryHandler::GetSolid(DB::Element^ elem) {
  DB::Options^			geom_opts;
  IEnumeratorGeometryObject^	geom_obj_it;
  DB::GeometryElement^		geom_elem;
  DB::GeometryInstance^		geom_inst;
  DB::Solid^			solid;

  geom_opts = gcnew DB::Options();
  geom_opts->DetailLevel = DB::ViewDetailLevel::Coarse;
  geom_opts->ComputeReferences = false;
  geom_opts->IncludeNonVisibleObjects = false;
  if ((geom_elem = elem->Geometry[geom_opts]) == nullptr)
    return (nullptr);
  geom_obj_it = geom_elem->GetEnumerator();
  while (geom_obj_it->MoveNext()) {
    if ((solid = dynamic_cast<DB::Solid^>(
            geom_obj_it->Current)) != nullptr && solid->Volume)
      return (solid);
    else if ((geom_inst = dynamic_cast<DB::GeometryInstance^>(
        geom_obj_it->Current)) != nullptr)
      return (GetSolid(geom_inst));
  }
  return (nullptr);
}

DB::XYZ^ RvtGeometryHandler::GetCentroid(DB::Element ^elem) {
  DB::Solid^			solid;

  if ((solid = GetSolid(elem)) != nullptr)
    return (solid->ComputeCentroid());
  return (nullptr);  
}

}  // namespace Utils
}  // namespace Samolet
