// Copyright 2019 Samolet LLC
// Author: kkozlov
// internal_storage.h

#ifndef INTERNAL_STORAGE_H_
# define INTERNAL_STORAGE_H_

namespace Samolet {
namespace internal_storage {

# define EXTRA_PARAMS_GEN_SCHEMA_GUID	Guid("2D721371-5984-4FF0-B514-4A49B1F1EB1D")
# define EXTRA_PARAMS_GEN_SCHEMA_NAME	"extra_params_data"
# define EXTRA_PARAMS_GEN_DATA_FIELD	"data"
# define VENDOR_ID			"com.samoletrvt.kkz"

# define GLOBAL_SCHEMA_GUID_STR_STR	Guid("A462F442-8343-43D6-9D21-47BFA560EEEF")
# define GLOBAL_DATA_STORAGE_STR_STR	"1AFE769C-4D77-4096-BB7A-D1966281C952"

# define TIMESTAMP_FIELD_STR		"timestamp"
# define DATA_FIELD_STR_STR		"data"  


// TODO(k.kozlov@samoletgroup.ru): replace with ContainerType
enum  FieldType { SIMPLE_FIELD, ARRAY_FIELD, MAP_FIELD };

public ref class FieldData {
public:
  FieldData(String^ a_field_name,
            FieldType a_field_type,
            Type^ a_storage_type)
      : field_name_{a_field_name},
        field_type_{a_field_type},
        storage_type_{a_storage_type},
        unit_type_{DB::UnitType::UT_Undefined},
        documentation_{"N/A"} {}
  
  String^ field_name(void) {
    return (field_name_);
  }
  FieldType field_type(void) {
    return (field_type_);
  }
  Type^	storage_type(void) {
    return (storage_type_);
  }
  Type^	key_storage_type(void) {
    return (key_storage_type_);
  }
  void	set_key_storage_type(Type^ type) {
    key_storage_type_ = type;
  }
  String^	documentation(void) {
    return (documentation_);
  }
  void	set_documentation(String^ a_desc) {
    documentation_ = a_desc;
  }
  DB::UnitType	unit_type(void) {
    return (unit_type_);
  }
  void	set_unit_type(DB::UnitType a_unit_type) {
    unit_type_ = a_unit_type;
  }
private:
  String^	field_name_;
  FieldType	field_type_;
  Type^		storage_type_;
  Type^		key_storage_type_;
  DB::UnitType	unit_type_;
  String^	documentation_;
};

public ref class SchemaData {
public:
  SchemaData(Guid a_guid, String^ a_schema_name,
             String^ a_vendor_id)
      : guid_{a_guid}, schema_name_{a_schema_name},
        vendor_id_{a_vendor_id}, documentation_{"N/A"},
        access_level_{ES::AccessLevel::Public},
        field_data_{gcnew CLG::LinkedList<FieldData^>()} {}
       
  Guid guid(void) {
    return (guid_);
  }
  String^ schema_name(void) {
    return (schema_name_);
  }
  ES::AccessLevel access_level(void) {
    return (access_level_);
  }
  void set_access_level(ES::AccessLevel access_lvl) {
    access_level_ = access_lvl;
  }
  String^ vendor_id(void) {
    return (vendor_id_);
  }
  String^ documentation(void) {
    return (documentation_);
  }
  void set_documentation(String ^a_desc) {
    documentation_ = a_desc;
  }
  void AddFieldData(FieldData^ fd){
    field_data_->AddFirst(fd);
  }
  CLG::IEnumerator<FieldData^>^ DataFieldEnumerator(void) {
    return (field_data_->GetEnumerator());
  }
 private:
  Guid				guid_;
  String^			schema_name_;
  ES::AccessLevel		access_level_;
  String^			vendor_id_;
  String^			documentation_;
  CLG::LinkedList<FieldData^>^	field_data_;
};

void	AddField(ES::SchemaBuilder^ schema_builder,
                 FieldData^ field_data);
ES::Schema^ ConstructSchema(SchemaData^ data);
}  // namespace internal_storage
namespace commands {
[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class ConstructSchemaCmd : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);
private:
    Type^ GetTypeFromString(String^ type_as_str);
    int TryParseTxtFile(String^ file_path,
                        Samolet::internal_storage::SchemaData^% out_schema_data);
    CLG::LinkedList<Samolet::internal_storage::SchemaData^>^
        TryParseTxtFile(String^ file_path);
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class CreateDataStorageElement : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);    
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class StoreDataInElemsCmd : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class RetrieveDataFromElemCmd : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class DeleteSchemaCmd : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class ListSchemasCmd : UI::IExternalCommand {
public:
    virtual UI::Result Execute(
        UI::ExternalCommandData ^cmdData,
        String ^%msg, DB::ElementSet ^elements);
};

} // namespace commands
} // namespace samoletrvt


#endif
