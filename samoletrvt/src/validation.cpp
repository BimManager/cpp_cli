// Copyright 2019 Samolet LLC
// Author: kkozlov
// ValidationCmd.cpp

#include "validation.h"

#include "web.h"
#include "params_data_parser.h"
#include "internal_storage.h"

namespace Samolet {
namespace Commands {
using namespace Validation;
array<System::String^>^ RetrieveParamNamesFromInternalStorage(DB::Document^ doc) {
  ES::Schema^			schema;
  ES::DataStorage^		data_storage;
  ES::Entity^			entity;
  IDictionaryStringString^	data;
  
  if ((schema = ES::Schema::Lookup(GLOBAL_SCHEMA_GUID_STR_STR)) == nullptr) {
    UI::TaskDialog::Show("Error", "Global schema is not defined.");
    return (nullptr);
  }
  if ((data_storage = Utils::RvtFilters::FindDataStorage(
          doc, GLOBAL_DATA_STORAGE_STR_STR)) == nullptr) {
    UI::TaskDialog::Show("Error", "Data storage is not defined.");
    return (nullptr);
  }
  entity = data_storage->GetEntity(schema);
  data = entity->Get<IDictionaryStringString^>(DATA_FIELD_STR_STR);
}



UI::Result ValidationCmd::Execute(
    UI::ExternalCommandData ^data,
    String ^%msg, DB::ElementSet ^elems) {
  IValidator	^validator;
  IValidationResult ^res;
  DB::Document		^doc;
  DB::Transaction	^tx;
  CL::IEnumerator ^it;

  doc = data->Application
      ->ActiveUIDocument->Document;
  validator = gcnew EmptyFieldsValidator(doc);
  /*  res = validator->Validate(Utils::RvtFilters
      ::FindAllQuantifiableElemIdsInView(
          doc, doc->ActiveView->Id)
          ->GetEnumerator());*/
  res = validator->Validate(
      Utils::RvtFilters::FindAllQuantifiableElemIds(doc)
      ->GetEnumerator());
  tx = gcnew DB::Transaction(doc);
  tx->Start("Isolate failed elements");
  doc->ActiveView->IsolateElementsTemporary(
      res->GetFailedIds());
  tx->Commit();
  return (UI::Result::Succeeded);
}
}  // namespace Commands

namespace Validation {
bool CarryOutValidation(DB::Document^ doc) {
  IEnumerator^		it;
  IValidator^		validator;
  IValidationResult^	result;
  array<IValidator^>^	validators;

  validators = gcnew array<IValidator^> {
    gcnew EmptyFieldsValidator(doc)
  };
  it = validators->GetEnumerator();
  while (it->MoveNext()) {
    validator = static_cast<IValidator^>(it->Current);
    result = validator->Validate(
      Utils::RvtFilters::FindAllQuantifiableElemIds(doc)
      ->GetEnumerator());
    if (!result->Valid)
      return (false);
  }
  return (true);
}

EmptyFieldsValidator::EmptyFieldsValidator(DB::Document ^doc)
    :doc_{doc} {}

DB::Document^ EmptyFieldsValidator::GetDocument(void) {
  return (doc_);
}

IValidationResult^ EmptyFieldsValidator::Validate(Object ^obj) {
  using Samolet::DataManagement::ParamDataParser;
  
  EmptyFieldsValidationResult^	validRes;
  DB::Element^			e;
  DB::Parameter^		p;
  CL::IEnumerator^		pIt;
  CL::IEnumerator^		eidIt;
  String^			val;
  String^			json_or_file_path;
  ParamDataParser^		parser;

  eidIt = static_cast<CL::IEnumerator^>(obj);
  validRes = gcnew EmptyFieldsValidationResult();
  validRes->Valid = true;
  parser = gcnew ParamDataParser();
  pIt = parser->ParameterNames()->GetEnumerator();
  while (eidIt->MoveNext()) {
    e = this->GetDocument()->GetElement(
        static_cast<DB::ElementId^>(eidIt->Current));
    while (pIt->MoveNext()) {
      if ((p = e->LookupParameter(
              static_cast<String^>(pIt->Current))) == nullptr)
        continue ;
      // if(!p->HasValue)
      //      val = Utils::RvtParamManager::GetValueAsString(p);
      //      if (val == nullptr || val == String::Empty) {
      if (!p->HasValue) {
        validRes->AddFailedId(e->Id);
        validRes->Valid = false;
        LOG(String::Format("{0} {1}",
                           e->Id->IntegerValue.ToString(),
                           static_cast<String^>(pIt->Current)));
      }
    }
    pIt->Reset();
  }
  LOG(validRes->GetFailedIds()->Count.ToString());
  return (validRes);
}

EmptyFieldsValidationResult::EmptyFieldsValidationResult(void)
    : _failedIds{gcnew CLG::LinkedList<DB::ElementId^>()} {}

void EmptyFieldsValidationResult::AddFailedId(DB::ElementId ^failedId) {
  if (_failedIds->Count == 0)
    this->_failedIds->AddFirst(failedId);
  else if (_failedIds->First->Value != failedId)
    this->_failedIds->AddFirst(failedId);
}

CLG::ICollection<DB::ElementId^>^ EmptyFieldsValidationResult::GetFailedIds(void) {
  return (this->_failedIds);
}
}  // namesapce Validation
}  // namespace Samolet


