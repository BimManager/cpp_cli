// Copyright Google LLC
// Author: kkozlov
// internal_storage.h

#include "internal_storage.h"

#include "gui_utils.h"
#include "utils.h"

namespace Samolet {
namespace internal_storage {
using namespace Autodesk::Revit::DB::ExtensibleStorage;
void	AddField(ES::SchemaBuilder^ schema_builder, FieldData^ field_data) {
  ES::FieldBuilder^	field_builder;

  switch (field_data->field_type()) {
    case FieldType::SIMPLE_FIELD:
      field_builder =
          schema_builder->AddSimpleField(
              field_data->field_name(),
              field_data->storage_type());
      break ;
    case FieldType::ARRAY_FIELD:
      field_builder =
          schema_builder->AddArrayField(
          field_data->field_name(),
          field_data->storage_type());
      break ;
    case FieldType::MAP_FIELD:
      field_builder = 
          schema_builder->AddMapField(
          field_data->field_name(),
          field_data->key_storage_type(),
          field_data->storage_type());
      break ;
  }
  if (field_builder->NeedsUnits())
    field_builder->SetUnitType(
        field_data->unit_type());
  field_builder->SetDocumentation(
      field_data->documentation());
}
                                                      
Schema^	ConstructSchema(SchemaData ^data) {
  SchemaBuilder^		schema_builder;
  FieldBuilder^			field_builder;
  ES::Schema^			schema;
  CLG::IEnumerator<FieldData^>^	it;

  if ((schema = Schema::Lookup(data->guid())) != nullptr)
    return (schema);
  schema_builder = gcnew SchemaBuilder(data->guid());
  schema_builder->SetSchemaName(data->schema_name());
  schema_builder->SetReadAccessLevel(data->access_level());
  schema_builder->SetWriteAccessLevel(data->access_level());
  schema_builder->SetVendorId(data->vendor_id());
  schema_builder->SetDocumentation(data->documentation());
  it = data->DataFieldEnumerator();
  while (it->MoveNext())
    AddField(schema_builder, it->Current);
  return (schema_builder->Finish());
}
} // namespace internal_storage

namespace commands {
namespace storage = Samolet::internal_storage;
using Samolet::internal_storage::FieldData;
using Samolet::internal_storage::SchemaData;
using Samolet::internal_storage::FieldType;
using DB::ExtensibleStorage::AccessLevel;
using DB::ExtensibleStorage::Schema;
using DB::ExtensibleStorage::Entity;

# define FILE_DOES_NOT_EXIST 1

Type^ ConstructSchemaCmd::GetTypeFromString(String^ type_as_str) {
  if (String::Compare("String", type_as_str) == 0)
    return (String::typeid);
  else if (String::Compare("Guid", type_as_str) == 0)
    return (Guid::typeid);
  else if (String::Compare("Int16", type_as_str) == 0)
    return (Int16::typeid);
  else if (String::Compare("Int32", type_as_str) == 0)
    return (Int32::typeid);
  else if (String::Compare("Float", type_as_str) == 0)
    return (float::typeid);
  else if (String::Compare("Double", type_as_str) == 0)
    return (Double::typeid);
  else if (String::Compare("ElementId", type_as_str) == 0)
    return (DB::ElementId::typeid);
  else if (String::Compare("XYZ", type_as_str) == 0)
    return (DB::XYZ::typeid);
  else if (String::Compare("UV", type_as_str) == 0)
    return (DB::UV::typeid);
  else if (String::Compare("Entity", type_as_str) == 0)
    return (Entity::typeid);
  return (nullptr);
}

int ConstructSchemaCmd::TryParseTxtFile(
    String^ file_path,
    storage::SchemaData^% out_schema_data) {
  IO::StreamReader^	sr;
  String^		line;
  array<String^>^	split_line;
  storage::FieldData^	field_data;
  
  sr = gcnew IO::StreamReader(file_path);
  while ((line = sr->ReadLine())) {
    split_line = line->Split();
    if (String::Compare(split_line[0], "S") == 0)
      out_schema_data = gcnew storage::SchemaData(
          Guid(split_line[1]), split_line[2], split_line[3]);
    else if (String::Compare(split_line[0], "F") == 0) {
      field_data = gcnew storage::FieldData(
          split_line[1], static_cast<storage::FieldType>(
              Int32::Parse(split_line[2])),
          GetTypeFromString(split_line[3]));
      if (split_line->Length == 5)
        field_data->set_key_storage_type(
            GetTypeFromString(split_line[4]));
      out_schema_data->AddFieldData(field_data);
    }
  }
  return (0);
}

CLG::LinkedList<storage::SchemaData^>^
ConstructSchemaCmd::TryParseTxtFile(String^ file_path) {
  IO::StreamReader^				sr;
  String^					line;
  array<String^>^				split_line;
  storage::FieldData^				field_data;
  CLG::LinkedList<storage::SchemaData^>^	ret_data;

  ret_data = gcnew CLG::LinkedList<storage::SchemaData^>();
  sr = gcnew IO::StreamReader(file_path);
  while ((line = sr->ReadLine())) {
    split_line = line->Split();
    if (String::Compare(split_line[0], "S") == 0) {
      ret_data->AddFirst(gcnew storage::SchemaData(
          Guid(split_line[1]), split_line[2], split_line[3]));
    }
    else if (String::Compare(split_line[0], "F") == 0) {
      field_data = gcnew storage::FieldData(
          split_line[1], static_cast<storage::FieldType>(
              Int32::Parse(split_line[2])),
          GetTypeFromString(split_line[3]));
      if (split_line->Length == 5)
        field_data->set_key_storage_type(
            GetTypeFromString(split_line[4]));
      ret_data->First->Value->AddFieldData(field_data);
    }
  }
  return (ret_data);
}

UI::Result ConstructSchemaCmd::Execute(
    UI::ExternalCommandData ^cmd_data,
    String ^%msg, DB::ElementSet ^elements) {
  Schema^				schema;
  String^				fp;
  CLG::IEnumerator<SchemaData^>^	sd_it;

  if ((fp = Samolet::Gui::GuiUtils::OpenFile(
          "Text Files(*.txt)|*.txt")) != nullptr) {
    sd_it = TryParseTxtFile(fp)->GetEnumerator();
    while (sd_it->MoveNext()) {
      schema = ConstructSchema(sd_it->Current);
#ifdef DEBUG
      Debug::WriteLine(String::Format(
             "Schema: {0} {1} has been constucted.",
             schema->SchemaName, schema->GUID.ToString()));
#endif
    }
    return (UI::Result::Succeeded);
  }  
  return (UI::Result::Failed);
}

UI::Result CreateDataStorageElement::Execute(
    UI::ExternalCommandData^ cmd_data,
    String^% msg, DB::ElementSet^ elems) {
  DB::Transaction^	tx;
  DB::Document^		doc;
  ES::DataStorage^	ds;

  doc = cmd_data->Application->ActiveUIDocument->Document;
  if ((ds = Utils::RvtFilters::FindDataStorage(
          doc, GLOBAL_DATA_STORAGE_STR_STR)) != nullptr) {
      UI::TaskDialog::Show("Info", String::Format(
      "Data storage element {0} already exists.",
      GLOBAL_DATA_STORAGE_STR_STR));
      return (UI::Result::Succeeded);
  }
  tx = gcnew DB::Transaction(doc);
  tx->Start("Create a data storage");
  ds = ES::DataStorage::Create(doc);
  ds->Name = GLOBAL_DATA_STORAGE_STR_STR;
  tx->Commit();
  UI::TaskDialog::Show("Info", String::Format(
      "Data storage element {0} has been generated.",
      GLOBAL_DATA_STORAGE_STR_STR));
  return (UI::Result::Succeeded);
}

UI::Result StoreDataInElemsCmd::Execute(
    UI::ExternalCommandData^ cmd_data,
    String^ %msg, DB::ElementSet^ elems) {
  Schema^		schema;
  DB::Transaction^	tx;
  UI::UIDocument^	uidoc;
  DB::Document^		doc;
  Entity^		entity;
  SEL::Selection^	selection;
  DB::Element^		picked_elem;
  DB::Parameter^	param;
  CLG::IDictionary<String^, String^>^	data;
  

  uidoc = cmd_data->Application->ActiveUIDocument;
  doc = uidoc->Document;
  selection = uidoc->Selection;
  picked_elem = doc->GetElement(
      selection->PickObject(SEL::ObjectType::Element,
                            "Pick an element"));
  if ((schema = Schema::Lookup(
          Guid(EXTRA_PARAMS_GEN_SCHEMA_GUID))) != nullptr) {
    param = picked_elem->LookupParameter("ExtraInfo");
    if (param != nullptr) {
      data = gcnew CLG::Dictionary<String^,String^>();
      data->Add("work_id", "1034423");
      entity = gcnew Entity(schema);
      entity->Set(EXTRA_PARAMS_GEN_DATA_FIELD, data);
      //      param->SetEntity(entity);
      tx = gcnew DB::Transaction(doc);
      tx->Start("Save data");
      picked_elem->SetEntity(entity);
      tx->Commit();
      UI::TaskDialog::Show("Info",
                           "The data have been saved in the element");
    }
    
  }
  return (UI::Result::Succeeded);
}

UI::Result RetrieveDataFromElemCmd::Execute(
    UI::ExternalCommandData^ cmd_data,
    String^ %msg, DB::ElementSet^ elems) {
  Schema^		schema;
  UI::UIDocument^	uidoc;
  DB::Document^		doc;
  Entity^		entity;
  SEL::Selection^	selection;
  DB::Element^		picked_elem;
  CLG::IDictionary<String^, String^>^	data;
  

  uidoc = cmd_data->Application->ActiveUIDocument;
  doc = uidoc->Document;
  selection = uidoc->Selection;
  picked_elem = doc->GetElement(
      selection->PickObject(SEL::ObjectType::Element,
                            "Pick an element"));
  if ((schema = Schema::Lookup(
          Guid(EXTRA_PARAMS_GEN_SCHEMA_GUID))) != nullptr) {
    entity = picked_elem->GetEntity(schema);
    if (entity->IsValid()) {
      data = entity->Get<CLG::IDictionary<String^,String^>^>(
          EXTRA_PARAMS_GEN_DATA_FIELD);
#ifdef DEBUG
      CL::IEnumerator^ it;
      String^ key;
      String^ val;

      it = data->Keys->GetEnumerator();
      while (it->MoveNext()) {
        key = static_cast<String^>(it->Current);
        if (data->TryGetValue(key, val)) {
          UI::TaskDialog::Show(
              "Info", String::Format(
                  "{0} = {1}", key, val));
        }
      }
#endif
    }
    
  }
  return (UI::Result::Succeeded);
}

UI::Result DeleteSchemaCmd::Execute(
    UI::ExternalCommandData^ cmd_data,
    String^ %msg, DB::ElementSet^ elems) {
  Schema^		schema;
  DB::Transaction^	tx;
  UI::UIDocument^	uidoc;
  DB::Document^		doc;
  

  uidoc = cmd_data->Application->ActiveUIDocument;
  doc = uidoc->Document;

  if ((schema = Schema::Lookup(
          Guid(EXTRA_PARAMS_GEN_SCHEMA_GUID))) != nullptr) {
      tx = gcnew DB::Transaction(doc);
      tx->Start("Erase schema from memory");
      Schema::EraseSchemaAndAllEntities(schema, true);
      tx->Commit();
      UI::TaskDialog::Show("Info",
                           "Schema has been erased from memory.");
  }
  return (UI::Result::Succeeded);
}

UI::Result ListSchemasCmd::Execute(
    UI::ExternalCommandData^ cmd_data,
    String^ %msg, DB::ElementSet^ elems) {
  CLG::IEnumerator<Schema^>^	it;
  UI::UIDocument^	uidoc;
  DB::Document^		doc;
  Schema^		schema;
  

  uidoc = cmd_data->Application->ActiveUIDocument;
  doc = uidoc->Document;
  it = Schema::ListSchemas()->GetEnumerator();
  while (it->MoveNext()) {
    schema = it->Current;
#ifdef DEBUG
    Debug::WriteLine(String::Format(
        "Schema: {0} {1} exists in memory.",
        schema->SchemaName, schema->GUID.ToString()));
#endif
  }
  return (UI::Result::Succeeded);
}

} // namespace commands
} // namespace samoletrvt
