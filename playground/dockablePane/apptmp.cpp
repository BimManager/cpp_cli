/*
 *  apptmp.cpp
 */

#include "apptmp.h"
#include "testPane.h"
#include "actionHandler.h"

using namespace Firm::Applications;
using namespace KKZ;

Autodesk::Revit::UI::Result
    Application::OnStartup(Autodesk::Revit::UI::UIControlledApplication ^app)
{
  TestPane				^pane;
  Autodesk::Revit::UI::DockablePaneId	^paneId;
  ActionHandler				^handler;

  handler = gcnew ActionHandler();
  pane = gcnew TestPane(handler);
  paneId = gcnew Autodesk::Revit::UI::DockablePaneId(System::Guid::NewGuid());
  app->RegisterDockablePane(paneId, "Foo", pane);
  app->Idling += gcnew EventHandler<
    Autodesk::Revit::UI::Events::IdlingEventArgs^>(this, &Application::OnIdling);
  return (Autodesk::Revit::UI::Result::Succeeded);
}

Autodesk::Revit::UI::Result
    Application::OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app)
{
    return (Autodesk::Revit::UI::Result::Succeeded);
}

void
Application::OnIdling(Object ^s, IdlingEventArgs ^e)
{
  AS::Application	^app;
  UI::UIApplication	^uiapp;
  UI::UIDocument	^uidoc;
  DB::Document		^doc;

  app = static_cast<AS::Application^>(s);
  uiapp = gcnew UI::UIApplication(app);
  uidoc = uiapp->ActiveUIDocument;
  doc = uidoc->Document;
#ifdef DEBUG
  Debug::WriteLine(
      String::Format("Title: {0}",
                     doc->Title));
#endif
}
