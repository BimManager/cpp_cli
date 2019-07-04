/*
 *  ribbon.h
 *  A template for creating 
 *  a custom tab and a ribbon
 */

#ifndef RIBBON_H
# define RIBBON_

# using <mscorlib.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define TAB_NAME       "ESTA"
# define RIBBON_NAME    "Esta Addins"
# define VM_DLL_PATH    "C:\\Users\\kkozlov\\AppData\\Roaming\\Autodesk\\Revit\\Addins\\2019\\viewsmgr.dll"
# define VM_BTN_NAME    "Views Manager"
# define VM_ENTRY_POINT "Esta.ViewsMgr.Command"

namespace Esta
{
    namespace UI = Autodesk::Revit::UI;
    namespace DB = Autodesk::Revit::DB;

    public ref class Ribbon : UI::IExternalApplication
    {
    public:
        virtual UI::Result  OnStartup(UI::UIControlledApplication ^app);
        virtual UI::Result  OnShutdown(UI::UIControlledApplication ^app);
    };
}

#endif