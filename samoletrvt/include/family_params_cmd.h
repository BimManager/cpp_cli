// Copyright 2019 Samolet LLC
// Author: kkozlov
// ParamsController.h

#ifndef PARAMS_CONTROLLER_H
# define PARAMS_CONTROLLER_H

namespace Samolet {
namespace Commands {
using System::Collections::Generic::ICollection;
using System::Collections::Generic::IEnumerator;
using Autodesk::Revit::DB::Element;
using Autodesk::Revit::DB::Document;
using Autodesk::Revit::DB::FilteredElementCollector;
using Autodesk::Revit::DB::FamilySymbol;
using Autodesk::Revit::DB::Family;
using Autodesk::Revit::DB::IFamilyLoadOptions;
using Autodesk::Revit::DB::FamilySource;
using Autodesk::Revit::DB::FamilyManager;
using Autodesk::Revit::DB::FamilyParameter;
using Autodesk::Revit::DB::ParameterType;
using Autodesk::Revit::DB::BuiltInParameterGroup;
using Autodesk::Revit::DB::Definition;

/* Forward declaration */
ref class FamilyParamDefinition;

delegate void ParameterModifierHandler(FamilyManager ^famMgr,
                                       FamilyParamDefinition ^paramDef);

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)] 
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class FamilyParameterRemoverCmd : UI::IExternalCommand {
public:
  virtual UI::Result Execute(UI::ExternalCommandData ^cmdData,
                             String ^%msg, DB::ElementSet ^elements);
};

public ref class FamilyParamDefinition {
 public:
  FamilyParamDefinition() {}
  FamilyParamDefinition(String ^name, DB::ParameterType type,
                        BuiltInParameterGroup group,
                        bool isInstance)
      : _name{name}, _paramType{type}, _paramGroup{group},
        _isInstance{isInstance} {}
  virtual property String^ Name {
    String^ get(void) {
      return (_name);
    }
  }
  virtual property DB::ParameterType ParameterType {
    DB::ParameterType get(void) {
      return (_paramType);
    }
  }
  virtual property BuiltInParameterGroup ParameterGroup {
    BuiltInParameterGroup get(void) {
      return (_paramGroup);
    }
  }
  virtual property bool IsInstance {
    bool get(void)  {
      return (_isInstance);
    }
  }
  virtual String^ ToString(void) override {
    return (String::Format(
        "{0};{1};{2};{3}",
        _name, _paramType,
        _paramGroup, _isInstance ? "Yes" : "No"));
  }
 private:
  String^		 	_name;
  DB::ParameterType		_paramType;
  BuiltInParameterGroup		_paramGroup;
  bool				_isInstance;
};

} // namespace Commands
} // namespace Samolet

#endif
