/*
 *  apptmp.cpp
 */

#include "apptmp.h"

using namespace Firm::Applications;

Autodesk::Revit::UI::Result
    Application::OnStartup(Autodesk::Revit::UI::UIControlledApplication ^app)
{
    return (Autodesk::Revit::UI::Result::Succeeded);
}

Autodesk::Revit::UI::Result
    Application::OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app)
{
    return (Autodesk::Revit::UI::Result::Succeeded);
}