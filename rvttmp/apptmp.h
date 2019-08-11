/*
 *  apptmp.h
 */

#ifndef APP_TMP_H
# define APP_TMP_H

# using <mscorlib.dll>
# using <System.dll>
# using <System.Configuration.dll>
# using <System.Reflection.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Firm
{
    using System::Object;
    using System::String;
    using System::Diagnostics::Debug;
    using System::Configuration::ConfigurationManager;
    using System::Configuration::Configuration;
    using System::Reflection::Assembly;

    namespace Applications
    {
        public ref class Application : Autodesk::Revit::UI::IExternalApplication
        {
        public:
            virtual Autodesk::Revit::UI::Result
                OnStartup(Autodesk::Revit::UI::UIControlledApplication ^app);
            virtual Autodesk::Revit::UI::Result
                OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app);
        };
    }
}

#endif