/*
 *  cmdtmp.h
 *  A template for working
 *  with the Revit API
 */

#ifndef RVT_TMP_H
# define RVT_TMP_H

# using <mscorlib.dll>
# using <System.dll>
# using <System.Configuration.dll>
# using <System.Reflection.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define GREETING_KEY "greeting"

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Firm
{
    using System::Object;
    using System::String;
    using System::Configuration::ConfigurationManager;
    using System::Configuration::Configuration;
    using System::Reflection::Assembly;
    using System::Diagnostics::Debug;

    int  GetValueFromConfigFile(String ^key, String ^%out);
    Object  ^GetValueFromConfigFile(String ^key, int %err);

    namespace Commands
    {
        [Autodesk::Revit::Attributes::TransactionAttribute(
            Autodesk::Revit::Attributes::TransactionMode::Manual)] /* ReadOnly */
        [Autodesk::Revit::Attributes::RegenerationAttribute(
            Autodesk::Revit::Attributes::RegenerationOption::Manual)] 
        [Autodesk::Revit::Attributes::JournalingAttribute(
            Autodesk::Revit::Attributes::JournalingMode::UsingCommandData)] /* NoCommandData */
        public ref class Command : Autodesk::Revit::UI::IExternalCommand
        {
        public:
            virtual Autodesk::Revit::UI::Result Execute(
                Autodesk::Revit::UI::ExternalCommandData ^cmdData,
                System::String ^%msg,
                Autodesk::Revit::DB::ElementSet ^elements);
        };
    }
}

#endif