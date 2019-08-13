/*
 *  apptmp.cpp
 */

#include "extevent.h"

using namespace Firm;
using namespace Firm::Events;
using namespace Firm::Applications;
using namespace Firm::Commands;
using namespace Firm::Gui;

void    ExtEventHandler::Execute(UIApplication ^uiapp)
{
#ifdef DEBUG
    Debug::WriteLine("[rvt] Execute");
#endif
    TaskDialog::Show("External Event", "Click Close to close.");
}

String    ^ExtEventHandler::GetName(void)
{
    return ("External Event Example");
}

Autodesk::Revit::UI::Result
    Application::OnStartup(Autodesk::Revit::UI::UIControlledApplication ^app)
{
#ifdef DEBUG
    Debug::WriteLine("Application has been registered.");
#endif
    this->_app =  this;
    return (Autodesk::Revit::UI::Result::Succeeded);
}

Autodesk::Revit::UI::Result
    Application::OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^app)
{
    if (this->_form != nullptr && this->_form->Visible)
        this->_form->Close();
#ifdef DEBUG
    Debug::WriteLine("The form has been destroyed.");
#endif
    return (Autodesk::Revit::UI::Result::Succeeded);
}

void    Application::ShowForm(UIApplication ^uiapp)
{
    if (this->_form == nullptr || this->_form->IsDisposed)
    {
        ExtEventHandler ^handler;
        ExternalEvent   ^extEvent;

        handler = gcnew ExtEventHandler();
        extEvent = ExternalEvent::Create(handler);
        this->_form = gcnew ModelessForm(extEvent, handler);
        this->_form->Show();
#ifdef DEBUG
    Debug::WriteLine("The form has popped up.");
#endif
    }
}

ModelessForm::ModelessForm(ExternalEvent ^extEvent, ExtEventHandler ^handler)
{
    this->_extEvent = extEvent;
    this->_handler = handler;
    InitializeComponent();
}

void    ModelessForm::InitializeComponent(void)
{
    Button  ^btn1;

    this->Size = System::Drawing::Size(300, 300);

    btn1 = gcnew Button();
    btn1->Text = "Close";
    btn1->Dock = DockStyle::Bottom;
    btn1->Click += gcnew EventHandler(this, &ModelessForm::OnCloseClicked);
    this->Controls->Add(btn1);
    btn1 = gcnew Button();
    btn1->Text = "Show";
    btn1->Dock = DockStyle::Top;
    btn1->Click += gcnew EventHandler(this, &ModelessForm::OnShowClicked);
    this->Controls->Add(btn1);
}

void    ModelessForm::OnShowClicked(Object ^s, EventArgs ^e)
{
    this->_extEvent->Raise();
}

void    ModelessForm::OnCloseClicked(Object ^s, EventArgs ^e)
{
    this->Close();
}

void    ModelessForm::OnFormClose(FormClosedEventArgs ^e)
{
    delete this->_extEvent;
    this->_extEvent = nullptr;
    this->_handler = nullptr;
}

Result Command::Execute(ExternalCommandData ^cmdData,
                String ^%msg, ElementSet ^elements)
{
#ifdef DEBUG
    Debug::WriteLine("Command has started");
#endif
    Application::GetApplication()->ShowForm(cmdData->Application);
    return (Result::Succeeded);
}