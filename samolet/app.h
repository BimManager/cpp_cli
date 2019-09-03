/*
 *  app.h
 */

#ifndef APP_H
# define APP_H

# include "stdafx.h"

# define TAB_NAME "Samolet"
# define PANEL_NAME "Development"

# define RS_NAME "icons"

/* #ifdef DEBUG
void    setupDebugging(void)
{
    System::Diagnostics::Debug::Listeners->Add(
        gcnew System::Diagnostics::TextWriterTraceListener(
            System::Console::Out));
}
#endif */

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
            void            AddButton(UI::RibbonPanel ^ribbon,
                                String ^iconName, String ^btnName,
                                String ^entryPnt);
        }; /* MainApp */
    } /* Applications */
}

#endif