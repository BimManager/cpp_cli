// Copyright Google LLC
// Author: kkozlov
// storage.h

#include "storage.h"

using namespace Firm;
using namespace Firm::Commands;

int  Firm::GetValueFromConfigFile(String ^key, String ^%out)
{
    Configuration   ^config;
    int             ret;

    config = ConfigurationManager::OpenExeConfiguration(
            Assembly::GetExecutingAssembly()->Location);
    if (config->HasFile)
    {  
        ret = config->AppSettings->Settings[key] != nullptr ? 0 : 1;
        if (!ret) 
            out = config->AppSettings->Settings[key]->Value;
    }
    else
        ret = 2;
    return (ret);
}

Object  ^Firm::GetValueFromConfigFile(String ^key, int %err)
{
    Configuration   ^config;

    config = ConfigurationManager::OpenExeConfiguration(
            Assembly::GetExecutingAssembly()->Location);
    if (config->HasFile)
    {  
        err = config->AppSettings->Settings[key] != nullptr ? 0 : 1;
        if (!err) 
            return ((String ^)config->AppSettings->Settings[key]->Value);
    }
    else
        err = 2;
    return (nullptr);
}

using namespace Autodesk::Revit::DB::ExtensibleStorage;
                                                      
enum FieldType { SIMPLE_FIELD, ARRAY_FIELD, MAP_FIELD };
enum StorageType { BOOLEAN, BYTE, INT, FLOAT, DOUBLE,
                   ELEMENT_ID, GUID, STRING, XYZ, UV, ENTITY };                                                     
                                                              
public ref class FieldData {
 public:
  FieldData(String^ aName, FieldType aFieldType,
            Type^ aStorageType)
      : _fieldName{aName}, _fieldType{aFieldType},
        _storageType{aStorageType}
  {}
  String^ GetFieldName(void) {
    return (_fieldName);
  }
  FieldType GetFieldType(void) {
    return (_fieldType);
  }
  Type ^GetStorageType(void) {
    return (_storageType);
  }
 private:
  String^	_fieldName;
  FieldType	_fieldType;
  Type^		_storageType;
};
 
public ref class SchemaData {
 public:
  SchemaData(Guid aGuid, String^ aName,
             FieldData^ aFieldData,
             String^ aVendorId)
      : _guid{aName}, _name{aName},
        _accessLevel{AccessLevel::Vendor},
        _vendorId{aVendorId}, _description{"N/A"},
        _fieldData{aFieldData}
  {}
  Guid GetGuid(void) {
    return (_guid);
  }
  String^ GetName(void) {
    return (_name);
  }
  AccessLevel GetAccessLevel(void) {
    return (_accessLevel);
  }
  String^ GetVendorId(void) {
    return (_vendorId);
  }
  String^ GetDescription(void) {
    return (_description);
  }
  void SetDescription(String ^aDesc) {
    _description = aDesc;
  }
  FieldData^ GetFieldData(void) {
    return (_fieldData);
  }
 private:
  Guid		_guid;
  String^	_name;
  AccessLevel	_accessLevel;
  String^	_vendorId;
  String^	_description;
  FieldData^	_fieldData;
};

void ConstructSchema(SchemaData ^data)
{
  SchemaBuilder	^schBldr;
  FieldBuilder	^fieldBldr;

  schBldr = gcnew SchemaBuilder(data->GetGuid());
  schBldr->SetSchemaName(data->GetName());
  schBldr->SetReadAccessLevel(data->GetAccessLevel());
  schBldr->SetWriteAccessLevel(data->GetAccessLevel());
  schBldr->SetVendorId(data->GetVendorId());
  schBldr->SetDocumentation(data->GetDescription());
}

Autodesk::Revit::UI::Result Command::Execute(
    Autodesk::Revit::UI::ExternalCommandData ^cmdData,
    System::String ^%msg,
    Autodesk::Revit::DB::ElementSet ^elements)
{
    return (Autodesk::Revit::UI::Result::Succeeded);
}
