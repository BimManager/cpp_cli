// Copyright 2019 Samolet LLC
// Author: kkozlov
// cmdtmp.h

#ifndef CMDTMP_H_
#define CMDTMP_H_

namespace Samolet {
namespace Commands {
[Autodesk::Revit::Attributes::TransactionAttribute(
    Autodesk::Revit::Attributes::TransactionMode::Manual)]
[Autodesk::Revit::Attributes::RegenerationAttribute(
    Autodesk::Revit::Attributes::RegenerationOption::Manual)]
[Autodesk::Revit::Attributes::JournalingAttribute(
    Autodesk::Revit::Attributes::JournalingMode::UsingCommandData)]
public ref class Command : Autodesk::Revit::UI::IExternalCommand {
public:
  virtual Autodesk::Revit::UI::Result Execute(
      Autodesk::Revit::UI::ExternalCommandData^ cmd_data,
      System::String^% msg, Autodesk::Revit::DB::ElementSet^ elements);
private:
  Autodesk::Revit::UI::Result ExecuteCmd(
      Autodesk::Revit::UI::ExternalCommandData^ cmd_data,
      System::String^% msg, Autodesk::Revit::DB::ElementSet^ elements);
};
}  // namespace Commands
}  // namespace Samolet

#endif  // INCLUDE_CMDTMP_H_
