/*
 *  stdafx.h
 */

 #ifndef STDAFX_H
 # define STDAFX_H

# using <System.dll>
# using <System.Configuration.dll>
#ifdef DEBUG
# using <System.Diagnostics.Debug.dll>
#endif

/* WinForms */
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

/* WPF */
# using <PresentationCore.dll>
# using <PresentationFramework.dll>
# using <WindowsBase.dll>
# using <System.Xaml.dll>

/* Revit API */
# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Samolet 
{
    using namespace System;
    namespace REF = System::Reflection;
    namespace RS = System::Resources;
    namespace IO = System::IO;

    namespace CF = System::Configuration;
    namespace CL = System::Collections;
    namespace CLG = System::Collections::Generic;
    
    #ifdef DEBUG
    using System::Diagnostics::Debug;
    #endif

    namespace Forms = System::Windows::Forms;
    namespace Dwg = System::Drawing;
    namespace IMG = System::Windows::Media::Imaging;
    //using System::Windows::Media::Imaging::BitmapImage;

    namespace DB = Autodesk::Revit::DB;
    namespace UI = Autodesk::Revit::UI;
    namespace AS = Autodesk::Revit::ApplicationServices;
    namespace ATT = Autodesk::Revit::Attributes;
    namespace SEL = UI::Selection;
}

 #endif