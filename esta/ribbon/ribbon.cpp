/*
 *  ribbon.cpp
 */

#include "ribbon.h"

namespace Esta
{
    UI::Result  Ribbon::OnStartup(UI::UIControlledApplication ^app)
    {
        LOG(PM_DLL_PATH);
        LOG(VM_DLL_PATH);
        InitializeRibbon(app);
        return (UI::Result::Succeeded);
    }

    UI::Result  Ribbon::OnShutdown(UI::UIControlledApplication ^app)
    {
        return (UI::Result::Succeeded);
    }

    void    Ribbon::InitializeRibbon(UI::UIControlledApplication ^app)
    {
        UI::RibbonPanel ^ribbon;

        app->CreateRibbonTab(TAB_NAME);
        ribbon = app->CreateRibbonPanel(TAB_NAME, RIBBON_NAME);

        AddButton(ribbon, VM_ICON, VM_BTN_NAME, VM_ENTRY_POINT, VM_DLL_PATH);
        AddButton(ribbon, PM_ICON, PM_BTN_NAME, PM_ENTRY_POINT, PM_DLL_PATH);
    }

    IO::MemoryStream ^Ribbon::GetMemoryStream(
        String ^asmName, String ^resName, String ^imgName)
    {
        ResourceManager ^rm;
        Assembly        ^asmb;

        asmb = Assembly::Load(asmName);
        rm = gcnew ResourceManager(resName, asmb);
        return (dynamic_cast<IO::MemoryStream ^>(rm->GetObject(imgName)));
    }

    BitmapImage       ^Ribbon::StreamToImage(IO::MemoryStream ^ms)
    {
        BitmapImage         ^icon;

        icon = gcnew BitmapImage();
        ms->Seek(0, IO::SeekOrigin::Begin);
        icon->BeginInit();
        icon->StreamSource = ms;
        icon->EndInit();
        ms->Close();
        return (icon);
    }

    void    Ribbon::AddButton(UI::RibbonPanel ^ribbon, String ^iconName,
                String ^btnName, String ^entpnt)
    {
        UI::PushButton      ^btn;
        IO::MemoryStream    ^ms;
        BitmapImage         ^icon;

        btn = (UI::PushButton ^)ribbon->
            AddItem(gcnew UI::PushButtonData(
                btnName, btnName, VM_DLL_PATH, entpnt));
        if ((ms = GetMemoryStream(Assembly::GetExecutingAssembly()
                ->GetName()->Name, RES_NAME, iconName)) != nullptr)
            if ((icon = StreamToImage(ms)) != nullptr)
                btn->LargeImage = icon;
    }

    void    Ribbon::AddButton(UI::RibbonPanel ^ribbon, String ^iconName,
                String ^btnName, String ^entpnt, String ^dllPath)
    {
        UI::PushButton      ^btn;
        IO::MemoryStream    ^ms;
        BitmapImage         ^icon;

        btn = (UI::PushButton ^)ribbon->
            AddItem(gcnew UI::PushButtonData(
                btnName, btnName, dllPath, entpnt));
        if ((ms = GetMemoryStream(Assembly::GetExecutingAssembly()
                ->GetName()->Name, RES_NAME, iconName)) != nullptr)
            if ((icon = StreamToImage(ms)) != nullptr)
                btn->LargeImage = icon;
    }
}