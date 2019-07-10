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

namespace Esta
{
    namespace ViewsMgr
    {
        namespace DGS = System::Diagnostics;
        namespace GCL = System::Collections::Generic;
        namespace DB = Autodesk::Revit::DB;
        namespace CL = System::Collections;

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

        ref class ViewData
        {
        public:
            ViewData(String ^name, String ^uniqueId,
                DB::ViewType viewType, char isOnSheet);

            String          ^GetName(void);
            String          ^GetUniqueId(void);
            DB::ViewType    GetViewType(void);
            char            IsOnSheet(void);

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
            void            Execute(void);
            void            DeleteElements(CL::IList ^viewNames);
        private:
            DB::Document    ^_doc;
            CL::SortedList  ^_namesIds;

            void    CollectAllViews(void);
            void    ConvertViewsData(
                        GCL::ICollection<DB::ElementId ^> ^viewIds);
        };

    }
}

#endif