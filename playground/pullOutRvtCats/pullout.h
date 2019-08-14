/*
 *  rvt_tmpl.h
 *  A template for working
 *  with the Revit API
 */

#ifndef RVT_TMPL_H
# define RVT_TMPL_H

# using <mscorlib.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace Rvt
{
    namespace CL = System::Collections;
    namespace TXT = System::Text;
    namespace IO = System::IO;
    namespace DB = Autodesk::Revit::DB;
    namespace UI = Autodesk::Revit::UI;

    typedef System::String              String;
    typedef System::IO::StreamWriter    StreamWriter;
    

    [Autodesk::Revit::Attributes::TransactionAttribute(
        Autodesk::Revit::Attributes::TransactionMode::Manual)]
    public ref class Command : Autodesk::Revit::UI::IExternalCommand
    {
    public:
        virtual Autodesk::Revit::UI::Result Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements);
            
    private:
        int PullOutCategories(DB::Document ^doc);
    };
}

#endif