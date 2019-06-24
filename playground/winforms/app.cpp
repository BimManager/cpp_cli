/*
 *  app.cpp
 */

#include "stdafx.h"
#include "form1.h"

#define EXIT_SUCCESS 0

[System::STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    System::Windows::Forms::Application::EnableVisualStyles();
    System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);

    System::Windows::Forms::Application::Run(gcnew Forms::Form1());
    return (EXIT_SUCCESS);
}
