// Copyright 2019 Samolet LLC
// Author: kkozlov
// cmdtmp.cpp

#include "cmdtmp.h"

namespace Samolet {
namespace Commands {
namespace UI = Autodesk::Revit::UI;
namespace DB = Autodesk::Revit::DB;

Autodesk::Revit::UI::Result
Command::Execute(Autodesk::Revit::UI::ExternalCommandData^ cmd_data,
                 System::String^% msg, Autodesk::Revit::DB::ElementSet^ elements) {
  return (ExecuteCmd(cmd_data, msg, elements));
}

Autodesk::Revit::UI::Result
Command::ExecuteCmd(Autodesk::Revit::UI::ExternalCommandData^ cmd_data,
                 System::String^% msg, Autodesk::Revit::DB::ElementSet^ elements) {
#ifdef DEBUG
  LOG(String::Format(
      "Executing Assembly: {0}.",
      System::Reflection::Assembly::GetExecutingAssembly()->FullName));
#endif
    return (Autodesk::Revit::UI::Result::Succeeded);
}

}  // namespace Commands
}  // namespace Samolet
