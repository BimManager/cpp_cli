/*
 *  app.cpp
 */

#include "app.h" 

using namespace Samolet;
using namespace Samolet::Applications;

Result  MainApp::OnStartup(UIControlledApplication ^uiapp)
{
    RibbonPanel ^panel;
#ifdef DEBUG
    Debug::WriteLine(String::Format("On Startup"));
#endif
    uiapp->CreateRibbonTab(TAB_NAME);
    panel = uiapp->CreateRibbonPanel(TAB_NAME, PANEL_NAME);
    this->AddButton(panel, "vm32x32", "Set Parameters", "Samolet.Commands.SetSpCmd");
    return (Result::Succeeded);
}

Result  MainApp::OnShutdown(UIControlledApplication ^uiapp)
{
    return (Result::Succeeded);
}

void    MainApp::AddButton(RibbonPanel ^ribbon,
            String ^iconName,
            String ^btnName, String ^entryPnt)
{
    using namespace Samolet::Resources;
    PushButton      ^btn;
    MemoryStream    ^ms;
    BitmapImage     ^icon;

    btn = (PushButton ^)ribbon->
        AddItem(gcnew PushButtonData(
            btnName, btnName, Assembly::GetExecutingAssembly()->Location, entryPnt));
    btn->AvailabilityClassName = "Samolet.Commands.AvailableUponSelection";
    if ((ms = Utils::GetMemoryStream(Assembly::GetExecutingAssembly(),
            RS_NAME, iconName)) != nullptr)
        if ((icon = Utils::MemoryStreamToImage(ms)) != nullptr)
            btn->LargeImage = icon;
}


System::IO::MemoryStream    ^Samolet::Resources::Utils::GetMemoryStream(
    Assembly ^asmb, String ^rsName, String ^imgName)
{
    ResourceManager ^rm;

    rm = gcnew ResourceManager(rsName, asmb);
    return (dynamic_cast<IO::MemoryStream ^>(rm->GetObject(imgName)));
}

BitmapImage ^Samolet::Resources::Utils::MemoryStreamToImage(
                MemoryStream ^ms)
{
    BitmapImage         ^icon;

    icon = gcnew BitmapImage();
    ms->Seek(0, SeekOrigin::Begin);
    icon->BeginInit();
    icon->StreamSource = ms;
    icon->EndInit();
    ms->Close();
    return (icon);
}
