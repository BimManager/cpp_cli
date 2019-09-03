/*
 *  app.cpp
 */

#include "app.h" 
#include "utils.h"

using namespace Samolet;
using namespace Samolet::Applications;

UI::Result  MainApp::OnStartup(UI::UIControlledApplication ^uiapp)
{
    UI::RibbonPanel ^panel;
#ifdef DEBUG
    Debug::WriteLine(String::Format("On Startup"));
#endif
    uiapp->CreateRibbonTab(TAB_NAME);
    panel = uiapp->CreateRibbonPanel(TAB_NAME, PANEL_NAME);
    this->AddButton(panel, "vm32x32", "Set Parameters", "Samolet.Commands.SetSpCmd");
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
    IMG::BitmapImage         ^icon;
    REF::Assembly       ^asmb;

    asmb = REF::Assembly::GetExecutingAssembly();
    btn = (UI::PushButton ^)ribbon->
        AddItem(gcnew UI::PushButtonData(
            btnName, btnName, asmb->Location, entryPnt));
    btn->AvailabilityClassName = "Samolet.Commands.AvailableUponSelection";
    if ((ms = Utils::ResourceManager::GetMemoryStream(asmb, RS_NAME, iconName)) != nullptr)
        if ((icon = Utils::ResourceManager::MemoryStreamToImage(ms)) != nullptr)
            btn->LargeImage = icon;
}