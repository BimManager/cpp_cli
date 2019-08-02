/*
 *  rvt_tmpl.h
 *  A template for working
 *  with the Revit API
 */

#ifndef RVT_TMPL_H
# define RVT_TMPL_H

# using <mscorlib.dll>
# using <System.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define IS_ON_SHEET(v) \
            v->Parameter[DB::BuiltInParameter::VIEWER_SHEET_NUMBER] \
             ->AsString() != "---" ? 1 : 0

# define LOG(s) System::Diagnostics::EventLog::WriteEntry("Application", s)

namespace Esta
{
    namespace ViewsMgr
    {
        namespace DGS = System::Diagnostics;
        namespace GCL = System::Collections::Generic;
        namespace DB = Autodesk::Revit::DB;
        namespace UI = Autodesk::Revit::UI;
        namespace CL = System::Collections;

        using System::Text::StringBuilder;
        typedef System::String  String;

        [Autodesk::Revit::Attributes::TransactionAttribute(
            Autodesk::Revit::Attributes::TransactionMode::Manual)]
        public ref class Command : Autodesk::Revit::UI::IExternalCommand
        {
        public:
            virtual Autodesk::Revit::UI::Result Execute(
                Autodesk::Revit::UI::ExternalCommandData ^cmdData,
                System::String ^%msg,
                Autodesk::Revit::DB::ElementSet ^elements);
        };

        ref class ViewData : System::IComparable
        {
        public:
            ViewData(String ^name, String ^uniqueId,
                DB::ViewType viewType, char isOnSheet);

            String          ^GetName(void);
            String          ^GetUniqueId(void);
            DB::ViewType    GetViewType(void);
            char            IsOnSheet(void);

            virtual int     CompareTo(System::Object ^obj);

        private:
            String          ^_name;
            String          ^_uniqueId;
            DB::ViewType    _viewType;
            char            _isOnSheet;
        };

        ref class ViewsMgr
        {
        public:
            ViewsMgr(DB::Document ^doc);

            CL::SortedList  ^GetNamesIds(void);
            CL::ArrayList    ^GetViewData(void);
            void            Execute(void);
            void            DeleteViews(CL::Hashtable ^uniqueIds);
            //void            DeleteElements(CL::IList ^viewNames);

        private:
            DB::Document    ^_doc;
            CL::SortedList  ^_namesIds;
            CL::ArrayList   ^_viewData;

            void    CollectAllViews(void);
            void    ConvertViewsData(
                        GCL::ICollection<DB::ElementId ^> ^viewIds);
            void    ViewsToViewData(GCL::ICollection<DB::ElementId ^> ^viewIds);
            void    LogViewData(void);
        };

    }
}

#endif