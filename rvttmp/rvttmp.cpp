/*
 *  rvttmp.cpp
 */

#include "rvttmp.h"

namespace Commands
{
    Autodesk::Revit::UI::Result Command::Execute(
        Autodesk::Revit::UI::ExternalCommandData ^cmdData,
        System::String ^%msg,
        Autodesk::Revit::DB::ElementSet ^elements)
        {
            String  ^greeting;

            greeting = ConfigurationManager::AppSettings[GREETING_KEY];
            if (greeting == nullptr)
                greeting = "No configuration file.";
            Autodesk::Revit::UI::TaskDialog::Show("Revit Template", greeting);
            return (Autodesk::Revit::UI::Result::Succeeded);
        }
}