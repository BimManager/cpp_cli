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
        IO::MemoryStream    ^ms;
        Bitmap          ^bmicon;
        BitmapImage     ^icon;

        app->CreateRibbonTab(TAB_NAME);
        ribbon = app->CreateRibbonPanel(TAB_NAME, RIBBON_NAME);
        btn = (UI::PushButton ^)ribbon->
            AddItem(gcnew UI::PushButtonData(
                VM_BTN_NAME, VM_BTN_NAME, VM_DLL_PATH, VM_ENTRY_POINT));
        ms = GetMemoryStream(Assembly::GetExecutingAssembly()
                ->GetName()->Name, RES_NAME, IMG_NAME);
        if (ms != nullptr)
        {
            LOG("bmicon is not nullptr");
            icon = StreamToImage(ms);
            if (icon != nullptr)
            {
                LOG(String::Format("H:{0} W:{1}", icon->PixelHeight, icon->PixelWidth));
                btn->LargeImage = icon;
            }
        }
        return (UI::Result::Succeeded);
    }

    UI::Result  Ribbon::OnShutdown(UI::UIControlledApplication ^app)
    {
        return (UI::Result::Succeeded);
    }

    BitmapImage ^Ribbon::GetBitmapImage(
        String ^asmName, String ^resName, String ^imgName)
    {
        ResourceManager ^rm;
        Assembly        ^asmb;

        asmb = Assembly::Load(asmName);
        rm = gcnew ResourceManager(resName, asmb);
        return (dynamic_cast<BitmapImage ^>(rm->GetObject(imgName)));
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

    Bitmap       ^Ribbon::GetBitmap(
        String ^asmName, String ^resName, String ^imgName)
    {
        ResourceManager ^rm;
        Assembly        ^asmb;
        Bitmap          ^img;

        asmb = Assembly::Load(asmName);
        rm = gcnew ResourceManager(resName, asmb);
        img = (Bitmap ^)rm->GetObject(imgName);
        return (img);
    }

    BitmapImage       ^Ribbon::BitmapToBitmapImage(Bitmap ^bitmap)
    {
        IO::MemoryStream    ^ms;
        BitmapImage         ^img;

        ms = gcnew IO::MemoryStream();
        bitmap->Save(ms, System::Drawing::Imaging::ImageFormat::Png);
        img = gcnew BitmapImage();
        ms->Seek(0, IO::SeekOrigin::Begin);
        img->BeginInit();
        img->StreamSource = ms;
        img->EndInit();
        ms->Close();
        return (img);
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
}