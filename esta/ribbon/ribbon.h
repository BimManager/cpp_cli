/*
 *  ribbon.h
 *  A template for creating 
 *  a custom tab and a ribbon
 */

#ifndef RIBBON_H
# define RIBBON_

# using <mscorlib.dll>

# using <System.Drawing.dll>
# using <PresentationCore.dll>
# using <WindowsBase.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define TAB_NAME       "ESTA"
# define RIBBON_NAME    "Esta Addins"

# define VM_DLL_PATH    "C:\\Users\\kkozlov\\AppData\\Roaming\\Autodesk\\Revit\\Addins\\2019\\viewsmgr.dll"
# define VM_BTN_NAME    "Views Manager"
# define VM_ENTRY_POINT "Esta.ViewsMgr.Command"

# define ASM_NAME   "ribbon.resources"
# define RES_NAME   "icons"
# define IMG_NAME   "viewsicon32x32"

namespace Esta
{
    typedef System::Drawing::Image              Image;
    typedef System::Reflection::Assembly        Assembly;
    typedef System::Resources::ResourceManager  ResourceManager;
    typedef System::Drawing::Bitmap             Bitmap;
    typedef System::String                      String;
    typedef System::Windows::Media::Imaging::BitmapImage     BitmapImage;

    namespace IO = System::IO;
    namespace UI = Autodesk::Revit::UI;
    namespace DB = Autodesk::Revit::DB;


    public ref class Ribbon : UI::IExternalApplication
    {
    public:
        virtual UI::Result  OnStartup(UI::UIControlledApplication ^app);
        virtual UI::Result  OnShutdown(UI::UIControlledApplication ^app);
    private:
        static Image       ^GetImage(
            String ^asmName, String ^resName, String ^imgName);
        static BitmapImage  ^ConvertBitmapToBitmapImage(Bitmap ^bitmap);
    };
}

#endif