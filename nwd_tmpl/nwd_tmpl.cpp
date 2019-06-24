/*
 *  nwd_tmpl.cpp
 */

#include "nwd_tmpl.h"

namespace Tmpl
{
    int NwdTmpl::Execute( ... array<String ^> ^parameters)
    {
        System::Windows::Forms::MessageBox::Show(
            Api::Application::Gui->MainWindow, "Hello, world!");
        return (0);
    }
}
