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
    namespace GCL = System::Collections::Generic;
    namespace DB = Autodesk::Revit::DB;
    namespace UI = Autodesk::Revit::UI;
    namespace CL = System::Collections;

    using System::Object;
    using System::String;
    using System::Text::StringBuilder;
    using CL::Hashtable;
    using CL::ArrayList;
    using CL::SortedList;
    using CL::IEnumerator;
    using GCL::ICollection;

    using DB::Document;
    using DB::View;
    using DB::ViewType;
    using DB::Element;
    using DB::Transaction;
    using DB::ElementCategoryFilter;
    using DB::FilteredElementCollector;

    namespace Commands
    {
        [Autodesk::Revit::Attributes::TransactionAttribute(
            Autodesk::Revit::Attributes::TransactionMode::Manual)]
        public ref class ViewsMgrCmd : Autodesk::Revit::UI::IExternalCommand
        {
        public:
            virtual Autodesk::Revit::UI::Result Execute(
                Autodesk::Revit::UI::ExternalCommandData ^cmdData,
                System::String ^%msg,
                Autodesk::Revit::DB::ElementSet ^elements);
        };
    }

    namespace ViewsMgnt
    {
        ref class ViewData : System::IComparable
        {
        public:
            ViewData(String ^name, String ^uniqueId,
                ViewType viewType, char isOnSheet);
            String          ^GetName(void);
            String          ^GetUniqueId(void);
            ViewType        GetViewType(void);
            char            IsOnSheet(void);
            virtual int     CompareTo(Object ^obj);

        private:
            String          ^_name;
            String          ^_uniqueId;
            ViewType        _viewType;
            char            _isOnSheet;
        };

        ref class ViewsMgr
        {
        public:
            ViewsMgr(DB::Document ^doc);

            SortedList      ^GetNamesIds(void);
            ArrayList       ^GetViewData(void);
            void            Execute(void);
            void            DeleteViews(Hashtable ^uniqueIds);

        private:
            Document    ^_doc;
            SortedList  ^_namesIds;
            ArrayList   ^_viewData;

            GCL::ICollection<Element ^>    ^CollectAllViews(void);
            void                            ViewsToViewData(
                                                GCL::ICollection<Element ^> ^views);
            void                            LogViewData(void);
        };

    }
}

#endif