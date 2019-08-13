/*
 *  wpfForm.h
 */

#ifndef WPF_FORM_H
# define WPF_FORM_H

# using <System.dll>

# using <WindowsBase.dll>
# using <PresentationCore.dll>
# using <PresentationFramework.dll>
# using <System.Xaml.dll>
# using <System.Xml.dll>

# include <windows.h>

using System::Object;
using System::String;
using System::IO::StreamReader;
using System::Windows::Window;
using System::Windows::Application;
using System::Windows::Markup::XamlReader;

[System::STAThread]
int WINAPI  WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmd);

#endif