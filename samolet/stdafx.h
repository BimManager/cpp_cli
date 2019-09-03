/*
 *  stdafx.h
 */

 #ifndef STDAFX_H
 # define STDAFX_H

# using <System.dll>
# using <System.Configuration.dll>

/* WinForms */
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

/* WPF */
# using <PresentationCore.dll>
# using <PresentationFramework.dll>
# using <WindowsBase.dll>
# using <System.Xaml.dll>
#ifdef DEBUG
# using <System.Diagnostics.Debug.dll>
#endif

/* Revit API */
# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

namespace Samolet 
{
    using namespace System;
    using namespace System::Reflection;
    using namespace System::Resources;
    using namespace System::IO;
    namespace CF = System::Configuration;

    namespace CL = System::Collections;
    using namespace System::Collections::Generic;
    #ifdef DEBUG
    using namespace System::Diagnostics;    
    #endif
    namespace DB = Autodesk::Revit::DB;
    namespace UI = Autodesk::Revit::UI;
    namespace SEL = UI::Selection;
    using namespace Autodesk::Revit::DB;
    using namespace Autodesk::Revit::UI;
    using namespace Autodesk::Revit::ApplicationServices;
    using namespace Autodesk::Revit::Attributes;

    namespace Forms = System::Windows::Forms;
    namespace Dwg = System::Drawing;
    using System::Windows::Media::Imaging::BitmapImage;
}

 #endif