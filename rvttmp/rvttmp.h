/*
 *  rvttmp.h
 *  A template for working
 *  with the Revit API
 */

#ifndef RVT_TMP_H
# define RVT_TMP_H

# using <mscorlib.dll>
# using <System.dll>
# using <System.Configuration.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define GREETING_KEY "greeting"

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Commands
{
    using System::String;
    using System::Configuration::ConfigurationManager;

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