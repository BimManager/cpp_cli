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
    using System::Windows::Media::Imaging::BitmapImage;

    namespace Applications
    {
        public ref class MainApp : IExternalApplication
        {
        public:
            virtual Result  OnStartup(UIControlledApplication ^uiapp);
            virtual Result  OnShutdown(UIControlledApplication ^uiapp);
        
        private:
            void            AddButton(RibbonPanel ^ribbon,
                                String ^iconName, String ^btnName,
                                String ^entryPnt);
        }; /* MainApp */
    } /* Applications */

    namespace Resources
    {
        using namespace System::IO;

        public ref class Utils
        {
        public:
        static MemoryStream ^GetMemoryStream(Assembly ^asmb,
                                String ^rsName, String ^imgName);
        static BitmapImage  ^MemoryStreamToImage(MemoryStream ^ms);
        }; /* Utils */
    } /* Resources */
}

#endif