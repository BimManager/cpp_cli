// Copyright 2019 Samolet LLC
// Author: kkozlov
// ValidationCmd.h

#ifndef VALIDATION_CMD_H
# define VALIDATION_CMD_H

# include "utils.h"

namespace Samolet {
namespace Validation {
public interface class IValidationResult {
  ICollectionElementId^			GetFailedIds(void);
  property bool 			Valid;
}; 

public interface class IValidator {
  Autodesk::Revit::DB::Document^	GetDocument(void); 
  IValidationResult^			Validate(Object ^o);
};

bool CarryOutValidation(DB::Document^ doc);

public ref class EmptyFieldsValidator :  IValidator {
public:
  EmptyFieldsValidator(DB::Document ^doc);

  virtual Autodesk::Revit::DB::Document^	GetDocument(void);
  virtual IValidationResult^ 			Validate(Object ^o);
private:
  Autodesk::Revit::DB::Document^		doc_;  
};

public ref class EmptyFieldsValidationResult : IValidationResult {
public:
  EmptyFieldsValidationResult(void);
  
  void AddFailedId(DB::ElementId ^failedId);
  virtual CLG::ICollection<DB::ElementId^>^ GetFailedIds(void);
  virtual property bool Valid;
private:
  LinkedListElementId^			_failedIds;
}; 
}  // namespace Validation
  
namespace Commands {
[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
public ref class ValidationCmd : UI::IExternalCommand {
public:
  virtual UI::Result Execute(
      UI::ExternalCommandData ^data,
      String ^%msg, DB::ElementSet ^elems);
}; 
}  // namespace Commands
}  // namespace Samolet

#endif
