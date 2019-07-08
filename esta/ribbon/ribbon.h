/*
 *  ribbon.h
 *  A template for creating 
 *  a custom tab and a ribbon
 */

#ifndef RIBBON_H
# define RIBBON_

# using <mscorlib.dll>

# using <System.dll>
# using <System.Drawing.dll>
# using <PresentationCore.dll>
# using <System.Xaml.dll>
# using <WindowsBase.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

# define TAB_NAME       "ESTA"
# define RIBBON_NAME    "Esta Addins"

# define VM_DLL_PATH    "C:\\Users\\kkozlov\\AppData\\Roaming\\Autodesk\\Revit\\Addins\\2019\\viewsmgr.dll"
# define VM_BTN_NAME    "Views Manager"
# define VM_ENTRY_POINT "Esta.ViewsMgr.Command"

# define ASM_NAME   "ribbon.resources"
# define RES_NAME   "icons"
# define IMG_NAME   "vm32x32"

namespace Esta
{
    typedef System::Drawing::Image                          Image;
    typedef System::Reflection::Assembly                    Assembly;
    typedef System::Resources::ResourceManager              ResourceManager;
    typedef System::Drawing::Bitmap                         Bitmap;
    typedef System::String                                  String;
    typedef System::Windows::Media::Imaging::BitmapImage    BitmapImage;
    typedef System::Windows::Media::Imaging::BitmapSource    BitmapSource;

    namespace IO = System::IO;
    namespace UI = Autodesk::Revit::UI;
    namespace DB = Autodesk::Revit::DB;

    public ref class Ribbon : UI::IExternalApplication
    {
    public:
        virtual UI::Result  OnStartup(UI::UIControlledApplication ^app);
        virtual UI::Result  OnShutdown(UI::UIControlledApplication ^app);
    private:
        static IO::MemoryStream ^GetMemoryStream(
            String ^asmName, String ^resName, String ^imgName);
        static BitmapImage  ^StreamToImage(IO::MemoryStream ^ms);            

        static Bitmap       ^GetBitmap(
            String ^asmName, String ^resName, String ^imgName);
        static BitmapImage  ^GetBitmapImage(
            String ^asmName, String ^resName, String ^imgName);
        static BitmapImage  ^BitmapToBitmapImage(Bitmap ^bitmap);
    };
}

#endif