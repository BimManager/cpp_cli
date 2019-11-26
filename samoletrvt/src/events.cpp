// Copyright 2019 Samolet LLC
// Author: kkozlov
// events.cpp

#include "events.h"

#include "utils.h"

namespace Samolet {
namespace Events {
namespace sys = System;
namespace db = Autodesk::Revit::DB;
namespace ui = Autodesk::Revit::UI;
namespace events = Autodesk::Revit::UI::Events;
namespace db_events = Autodesk::Revit::DB::Events;
namespace app_services = Autodesk::Revit::ApplicationServices;
using System::Diagnostics::Process;
using System::Diagnostics::ProcessStartInfo;

void EventsManager::CopyNewVerAsmblOnClosing(
    Object ^s, EVT::ApplicationClosingEventArgs ^e){
  String		^outValue;
  String		^src;
  String		^dst;
  ProcessStartInfo	^startInfo;
  array<Process^>	^runningRvts;
  CL::IEnumerator	^it;

  runningRvts = Process::GetProcessesByName("revit");
  if (runningRvts->Length == 1
      && !Utils::ConfigFileManager::GetValueFromConfigFile(
          "updaterPath", outValue)) {
    if (!IO::File::Exists(outValue))
      return ;
    dst = IO::Path::GetDirectoryName(
        REF::Assembly::GetExecutingAssembly()->Location);
    startInfo = gcnew ProcessStartInfo(outValue);
    startInfo->UseShellExecute = false;
    startInfo->CreateNoWindow = true;
    if (!Utils::ConfigFileManager::GetValueFromConfigFile(
            "filesToCopy", outValue)
        && !Utils::ConfigFileManager::GetValueFromConfigFile(
            "sourcePath", src)) {
      it = outValue->Split(':')->GetEnumerator();
      while (it->MoveNext()) {
        startInfo->Arguments =
            String::Format("{0}\\{1} {2}\\{3}",
                         src,(String^)it->Current,
                         dst, static_cast<String^>(it->Current));
        Process::Start(startInfo);
      }
    }
  }
#ifdef DEBUG
  if (runningRvts->Length > 1)
    Debug::WriteLine
      ("Other instances of Revit are running.");
  Debug::WriteLine
      ("All Revit instances have terminated.");
#endif
}

void EventsManager::OnIdling(sys::Object^ s, events::IdlingEventArgs^ e) {
  app_services::Application^	app;
  ui::UIApplication^		uiapp;
  ui::UIDocument^		uidoc;
  db::Document^			doc;

  if ((app = static_cast<app_services::Application^>(s)) != nullptr) {
    uiapp = gcnew ui::UIApplication(app);
    uidoc = uiapp->ActiveUIDocument;
    doc = uidoc->Document;
    LOG(doc->Title);
  } else {
#ifdef DEBUG
  Debug::Assert(app != nullptr, "Application is invalid.");
#endif
  }
}

void EventsManager::OnApplicationInitialized(
    sys::Object^ s,
    db_events::ApplicationInitializedEventArgs^ e) {
  
}

void EventsManager::OnDialogBoxShowing(
    System::Object^ s,
    Autodesk::Revit::UI::Events::DialogBoxShowingEventArgs^ e) {
  namespace events = Autodesk::Revit::UI::Events;

  events::MessageBoxShowingEventArgs^	msgbox_e;
  events::TaskDialogShowingEventArgs^	taskdlg_e;

#ifdef DEBUG
  /*  LOG(String::Format("{0} has been dismissed.", e->DialogId));
  if ((msgbox_e = dynamic_cast<events::MessageBoxShowingEventArgs^>(e)) != nullptr) {
    LOG(String::Concat("Message box ", msgbox_e->Message));
    msgbox_e->OverrideResult(0); // MB_OK
  }
  else if ((taskdlg_e = dynamic_cast<events::TaskDialogShowingEventArgs^>(e)) != nullptr) {
    LOG(String::Concat("Task dialog: ", taskdlg_e->Message));
    taskdlg_e->OverrideResult(0); // MB_OK
  }
  else {
    LOG(String::Concat("Dialog box: ", e->DialogId));
    e->OverrideResult(1); // any non-zero value
  }*/
#endif
}
}  // namespace events
}  // namespace samoletrvt
