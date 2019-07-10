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
# define RES_NAME       "icons"

# define VM_DLL_PATH    System::String::Format("{0}\\Autodesk\\Revit\\Addins\\2019\\viewsmgr.dll", \
                            System::Environment::GetFolderPath(System::Environment::SpecialFolder::ApplicationData))
# define VM_BTN_NAME    "Views Manager"
# define VM_ENTRY_POINT "Esta.ViewsMgr.Command"
# define VM_ICON    "vm32x32"


# define PM_DLL_PATH    System::String::Format("{0}\\Autodesk\\Revit\\Addins\\2019\\paramsmgr.dll", \
                            System::Environment::GetFolderPath(System::Environment::SpecialFolder::ApplicationData))
# define PM_BTN_NAME    "Params Manager"
# define PM_ENTRY_POINT "Esta.Command"
# define PM_ICON    "pm32x32"

namespace Esta
{
    typedef System::Drawing::Image                          Image;
    typedef System::Reflection::Assembly                    Assembly;
    typedef System::Resources::ResourceManager              ResourceManager;
    typedef System::Drawing::Bitmap                         Bitmap;
    typedef System::String                                  String;
    typedef System::Windows::Media::Imaging::BitmapImage    BitmapImage;
    typedef System::Windows::Media::Imaging::BitmapSource   BitmapSource;

    namespace IO = System::IO;
    namespace UI = Autodesk::Revit::UI;
    namespace DB = Autodesk::Revit::DB;

    public ref class Ribbon : UI::IExternalApplication
    {
    public:
        virtual UI::Result  OnStartup(UI::UIControlledApplication ^app);
        virtual UI::Result  OnShutdown(UI::UIControlledApplication ^app);
        static void         InitializeRibbon(UI::UIControlledApplication ^app);
        static void         AddButton(UI::RibbonPanel ^ribbon, String ^iconName,
                                String ^btnName, String ^entpnt);
        static void         AddButton(UI::RibbonPanel ^ribbon, String ^iconName,
                                String ^btnName, String ^entpnt, String ^dllPath);
        
    private:
        static IO::MemoryStream ^GetMemoryStream(
            String ^asmName, String ^resName, String ^imgName);
        static BitmapImage  ^StreamToImage(IO::MemoryStream ^ms);            
    };
}

#endif