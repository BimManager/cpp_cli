/*
 *  extevent.h
 */

#ifndef APP_TMP_H
# define APP_TMP_H

# using <mscorlib.dll>
# using <System.dll>
# using <System.Configuration.dll>
# using <System.Reflection.dll>

# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Firm
{
    using System::Object;
    using System::EventArgs;
    using System::EventHandler;
    using System::String;
    using System::Diagnostics::Debug;
    using System::Configuration::ConfigurationManager;
    using System::Configuration::Configuration;
    using System::Reflection::Assembly;

    using System::Windows::Forms::Form;
    using System::Windows::Forms::FormClosedEventArgs;
    using System::Windows::Forms::Button;
    using System::Windows::Forms::DockStyle;

    using Autodesk::Revit::UI::UIApplication;
    using Autodesk::Revit::UI::TaskDialog;
    using Autodesk::Revit::UI::ExternalEvent;
    using Autodesk::Revit::UI::IExternalEventHandler;

    namespace Events
    {
        public ref class ExtEventHandler : IExternalEventHandler
        {
        public:
            virtual void    Execute(UIApplication ^uiapp);
            virtual String  ^GetName(void);
        };
    }

    namespace Gui
    {
        using namespace Events;

        public ref class ModelessForm : Form
        {
        public:
            ModelessForm(ExternalEvent ^extEvent, ExtEventHandler ^handler);
        protected:
            virtual void    OnFormClose(FormClosedEventArgs ^e);
        private:
            ExternalEvent   ^_extEvent;
            ExtEventHandler ^_handler;

            void    InitializeComponent(void);
            void    OnCloseClicked(Object ^s, EventArgs ^e);
            void    OnShowClicked(Object ^s, EventArgs ^e);
        };
    }

    namespace Applications
    {
        using namespace Gui;

        public ref class Application : Autodesk::Revit::UI::IExternalApplication
        {
        public:
            virtual Autodesk::Revit::UI::Result
                OnStartup(Autodesk::Revit::UI::UIControlledApplication ^app);
            virtual Autodesk::Revit::UI::Result
                OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app);
            void    ShowForm(UIApplication ^uiapp);
            static Application  ^GetApplication(void)
            {
                return (_app);
            }
        private:
            ModelessForm            ^_form;
            static Application     ^_app;
        };
    }

    namespace Commands
    {
        using Autodesk::Revit::UI::Result;
        using Autodesk::Revit::Attributes::TransactionAttribute;
        using Autodesk::Revit::Attributes::TransactionMode;
        using Autodesk::Revit::UI::IExternalCommand;
        using Autodesk::Revit::UI::ExternalCommandData;
        using Autodesk::Revit::DB::ElementSet;

        [TransactionAttribute(TransactionMode::Manual)]
        public ref class Command : IExternalCommand
        {
        public:
            virtual Result Execute(ExternalCommandData ^cmdData,
                String ^%msg, ElementSet ^elements);
        };
    }
}

#endif