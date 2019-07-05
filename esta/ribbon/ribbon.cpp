/*
 *  ribbon.cpp
 */

#include "ribbon.h"

namespace Esta
{
    UI::Result  Ribbon::OnStartup(UI::UIControlledApplication ^app)
    {
        UI::RibbonPanel ^ribbon;
        UI::PushButton  ^btn;
        Bitmap          ^icon;
        BitmapImage     ^img;

        app->CreateRibbonTab(TAB_NAME);
        ribbon = app->CreateRibbonPanel(TAB_NAME, RIBBON_NAME);
        btn = (UI::PushButton ^)ribbon->
            AddItem(gcnew UI::PushButtonData(
                VM_BTN_NAME, VM_BTN_NAME, VM_DLL_PATH, VM_ENTRY_POINT));
        icon = gcnew Bitmap(Ribbon::GetImage(ASM_NAME, RES_NAME, IMG_NAME));
        if (icon != nullptr)
        {
            img = Ribbon::ConvertBitmapToBitmapImage(icon);
            btn->LargeImage = img;
        }
        return (UI::Result::Succeeded);
    }

    UI::Result  Ribbon::OnShutdown(UI::UIControlledApplication ^app)
    {
        return (UI::Result::Succeeded);
    }

    Image       ^Ribbon::GetImage(
        String ^asmName, String ^resName, String ^imgName)
    {
        ResourceManager ^rm;
        Assembly        ^asmb;
        Image           ^img;

        asmb = Assembly::Load(asmName);
        rm = gcnew ResourceManager(resName, asmb);
        img = (Image ^)rm->GetObject(imgName);
        return (img);
    }

    BitmapImage       ^Ribbon::ConvertBitmapToBitmapImage(Bitmap ^bitmap)
    {
        IO::MemoryStream    ^ms;
        BitmapImage         ^img;

        ms = gcnew IO::MemoryStream();
        bitmap->Save(ms, System::Drawing::Imaging::ImageFormat::Png);
        img = gcnew BitmapImage();
        img->BeginInit();
        ms->Seek(0, IO::SeekOrigin::Begin);
        img->StreamSource = ms;
        img->EndInit();
        ms->Close();
        return (img);
    }
}