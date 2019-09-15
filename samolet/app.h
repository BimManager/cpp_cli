/*
 *  app.h
 */

#ifndef APP_H
# define APP_H

# include "stdafx.h"

# define TAB_NAME "Samolet"
# define PANEL_NAME "Development"

# define RS_NAME "icons"

namespace Samolet
{
    namespace Applications
    {
        public ref class MainApp : UI::IExternalApplication
        {
        public:
            virtual UI::Result  OnStartup(UI::UIControlledApplication ^uiapp);
            virtual UI::Result  OnShutdown(UI::UIControlledApplication ^uiapp);
        private:
            void                AddButton(UI::RibbonPanel ^ribbon,
                                    String ^iconName, String ^btnName,
                                    String ^entryPnt);
            UI::PushButtonData  ^CreatePushButton(String ^nameAndText,
                                    String ^iconName, String ^className);
            void                AddExtraInfo(UI::PushButtonData ^btnData,
                                    String ^avblClass, String ^toolTip);
        }; /* MainApp */
    } /* Applications */
}

#endif