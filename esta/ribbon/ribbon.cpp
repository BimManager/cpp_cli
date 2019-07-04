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

        app->CreateRibbonTab(TAB_NAME);
        ribbon = app->CreateRibbonPanel(TAB_NAME, RIBBON_NAME);
        btn = (UI::PushButton ^)ribbon->
            AddItem(gcnew UI::PushButtonData(
                VM_BTN_NAME, VM_BTN_NAME, VM_DLL_PATH, VM_ENTRY_POINT));

        return (UI::Result::Succeeded);
    }

    UI::Result  Ribbon::OnShutdown(UI::UIControlledApplication ^app)
    {
        return (UI::Result::Succeeded);
    }
}