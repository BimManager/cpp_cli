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
  return (Autodesk::Revit::UI::Result::Succeeded);
}

Autodesk::Revit::UI::Result
    Application::OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app)
{
    return (Autodesk::Revit::UI::Result::Succeeded);
}
