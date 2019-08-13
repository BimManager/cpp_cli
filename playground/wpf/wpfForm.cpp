
/*
 *  wpfForm.cpp
 */

#include "wpfForm.h"

int WINAPI  WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmd)
//int main(void)
{
    StreamReader    ^sr;
    Window          ^win;
    Application     ^app;
    String          ^xamlAsStr;

    sr = gcnew StreamReader("mainWindow.xaml");
    xamlAsStr = sr->ReadToEnd();
    sr->Close();
    //System::Console::Write(xamlAsStr);
    win = static_cast<Window ^>(XamlReader::Parse(xamlAsStr));
    //win = gcnew Window();
    //win->Title = "Hello World";
    app = gcnew Application();
    app->Run(win);
    return (0);
}