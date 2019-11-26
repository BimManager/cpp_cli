// Copyright 2019 Samolet LLC
// Author: kkozlov
// events.h

#ifndef EVENTS_H_
# define EVENTS_H_

namespace Samolet {
namespace Events {
public ref class EventsManager {
public:
  static void CopyNewVerAsmblOnClosing(
      Object ^s, EVT::ApplicationClosingEventArgs ^e);
  static void OnIdling(System::Object^ s,
                       Autodesk::Revit::UI::Events::IdlingEventArgs^ e);
  static void OnApplicationInitialized(
      System::Object^ s,
      Autodesk::Revit::DB::Events::ApplicationInitializedEventArgs^ e);
  static void OnDialogBoxShowing(
      System::Object^ s,
      Autodesk::Revit::UI::Events::DialogBoxShowingEventArgs^ e);
};
}  // namespace events
}  // namespace samoletrvt
#endif
