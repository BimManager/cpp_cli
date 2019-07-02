/*
 *  rvt_tmpl.cpp
 */

#include "viewsmgr.h"
#include "viewsmgr_form.h"

namespace Esta
{
    namespace ViewsMgr
    {
        using namespace Gui;

        Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                ViewsMgrForm    ^form;

                form = gcnew ViewsMgrForm();
                form->ShowDialog();
                return (Autodesk::Revit::UI::Result::Succeeded);
            }
    }
}