/*
 *  rvt_tmpl.cpp
 */

#include "rvt_tmpl.h"

namespace Rvt
{
    Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                Autodesk::Revit::UI::TaskDialog::Show("Revit Template", "C++ rocks.");
                return (Autodesk::Revit::UI::Result::Succeeded);
            }
}