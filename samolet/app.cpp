/*
 *  app.cpp
 */

#include "app.h" 
#include "utils.h"

using namespace Samolet;
using namespace Samolet::Applications;

UI::Result  MainApp::OnStartup(UI::UIControlledApplication ^uiapp)
{
    UI::RibbonPanel     ^panel;
    UI::PushButtonData  ^btnData;

#ifdef DEBUG
    Debug::WriteLine(String::Format("On Startup"));
#endif
    uiapp->CreateRibbonTab(TAB_NAME);
    panel = uiapp->CreateRibbonPanel(TAB_NAME, PANEL_NAME);
    btnData = CreatePushButton("Select Values", "vm32x32", "Samolet.Commands.SetSpCmd");
    this->AddExtraInfo(btnData, "Samolet.Commands.AvailableUponSelection", "ToolTip");
    panel->AddItem(btnData);
    return (UI::Result::Succeeded);
}

UI::Result  MainApp::OnShutdown(UI::UIControlledApplication ^uiapp)
{
    uiapp = nullptr;
    return (UI::Result::Succeeded);
}

void    MainApp::AddButton(UI::RibbonPanel ^ribbon,
            String ^iconName,
            String ^btnName, String ^entryPnt)
{
    UI::PushButton      ^btn;
    IO::MemoryStream    ^ms;
    IMG::BitmapImage    ^icon;
    REF::Assembly       ^asmb;

    asmb = REF::Assembly::GetExecutingAssembly();
    btn = (UI::PushButton ^)ribbon->
        AddItem(gcnew UI::PushButtonData(
            btnName, btnName, asmb->Location, entryPnt));
    //btn->AvailabilityClassName = "Samolet.Commands.AvailableUponSelection";
    if ((ms = Utils::ResourceManager::GetMemoryStream(asmb, RS_NAME, iconName)) != nullptr)
        if ((icon = Utils::ResourceManager::MemoryStreamToImage(ms)) != nullptr)
            btn->LargeImage = icon;
}

UI::PushButtonData  ^MainApp::CreatePushButton(String ^nameAndText,
                        String ^iconName, String ^className)
{
    UI::PushButtonData  ^btnData;
    REF::Assembly       ^asmb;
    IO::MemoryStream    ^ms;
    IMG::BitmapImage    ^icon;

    asmb = REF::Assembly::GetExecutingAssembly();
    btnData = gcnew UI::PushButtonData(nameAndText, nameAndText, asmb->Location, className);
    if ((ms = Utils::ResourceManager::GetMemoryStream(asmb, RS_NAME, iconName)) != nullptr)
        if ((icon = Utils::ResourceManager::MemoryStreamToImage(ms)) != nullptr)
            btnData->LargeImage = icon;
    return (btnData);
}

void    MainApp::AddExtraInfo(UI::PushButtonData ^btnData,
            String ^avblClass, String ^toolTip)
{
    btnData->AvailabilityClassName = avblClass;
    btnData->ToolTip = toolTip;
}