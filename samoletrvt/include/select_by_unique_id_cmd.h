// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_by_unique_id_cmd.h

#ifndef SELECT_BY_UNIQE_ID_CMD_H_
#define SELECT_BY_UNIQUE_ID_CMD_H_

namespace Samolet {
namespace Commands {
[Autodesk::Revit::Attributes::TransactionAttribute(
    Autodesk::Revit::Attributes::TransactionMode::Manual)]
[Autodesk::Revit::Attributes::RegenerationAttribute(
    Autodesk::Revit::Attributes::RegenerationOption::Manual)] 
[Autodesk::Revit::Attributes::JournalingAttribute(
    Autodesk::Revit::Attributes::JournalingMode::UsingCommandData)]
public ref class SelectByUniqueId : Autodesk::Revit::UI::IExternalCommand {
public:
  virtual Autodesk::Revit::UI::Result Execute(
      Autodesk::Revit::UI::ExternalCommandData ^cmdData,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elements);
private:
  ICollectionElementId^	SelectElemsByUniqueId(
      Autodesk::Revit::UI::UIDocument ^uidoc, ICollectionString^ guids);
  ICollectionString^	CheckUniqueIds(array<System::String^>^ guids);
};  
}  // namespace Commands
}  // namespace Samolet

#endif  // SELECT_BY_UNIQUE_ID_CMD_h
