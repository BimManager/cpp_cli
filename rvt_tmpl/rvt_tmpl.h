/*
 *  rvt_tmpl.h
 *  A template for working
 *  with the Revit API
 */

#ifndef RVT_TMPL_H
# define RVT_TMPL_H

# using <mscorlib.dll>
# using <System.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Rvt
{
    [Autodesk::Revit::Attributes::TransactionAttribute(
        Autodesk::Revit::Attributes::TransactionMode::Manual)]
    public ref class Command : Autodesk::Revit::UI::IExternalCommand
    {
    public:
        virtual Autodesk::Revit::UI::Result Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements);
    };
}

#endif