/*
 *  rvt_tmpl.cpp
 */

#include "viewsmgr.h"
#include "viewsmgr_form.h"

namespace Esta
{
    namespace ViewsMgr
    {
        using namespace Gui;
        namespace DGS = System::Diagnostics;

        Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                ViewsMgrForm    ^form;
                ViewsMgr        ^mgr;
                DB::Transaction ^tr;
                DB::Document    ^doc;

                doc = cmdData->Application->ActiveUIDocument->Document;
                mgr = gcnew ViewsMgr(doc);
                mgr->Execute();
                form = gcnew ViewsMgrForm(mgr->GetNamesIds()->GetKeyList());
                form->ShowDialog();
                tr = gcnew DB::Transaction(doc);
                if (form->GetCheckedIndices() != nullptr)
                {
                    tr->Start("Delete views");
                    mgr->DeleteElements(form->GetCheckedIndices());
                    tr->Commit();
                }
                return (Autodesk::Revit::UI::Result::Succeeded);
            }

        ViewsMgr::ViewsMgr(DB::Document ^doc)
        {
            this->_doc = doc;
        }

        CL::SortedList  ^ViewsMgr::GetNamesIds(void)
        {
            return (this->_namesIds);
        }

        void    ViewsMgr::Execute(void)
        {
            this->CollectAllViews();
        }

        void    ViewsMgr::CollectAllViews(void)
        {
            DB::FilteredElementCollector        ^col;
            DB::ElementCategoryFilter           ^catFltr;
            GCL::ICollection<DB::ElementId ^>   ^viewIds;
            CL::IEnumerator                     ^it;

            col = gcnew DB::FilteredElementCollector(this->_doc);
            catFltr = gcnew DB::ElementCategoryFilter(DB::BuiltInCategory::OST_Views);
            viewIds = col->WherePasses(catFltr)
                         ->WhereElementIsNotElementType()
                         ->ToElementIds();
            this->ConvertViewsData(viewIds);
            this->ViewsToViewData(viewIds);
            this->LogViewData();
        }

        /* name;id;notOnSheet;ViewType[Legend] */
        void    ViewsMgr::ConvertViewsData(
                    GCL::ICollection<DB::ElementId ^> ^viewIds)
        {
            CL::IEnumerator ^it;
            DB::View        ^v;
            DB::Parameter   ^p;

            this->_namesIds = gcnew CL::SortedList(
                gcnew CL::CaseInsensitiveComparer());
            it = viewIds->GetEnumerator();
            while (it->MoveNext())
            {
                v = dynamic_cast<DB::View ^>(
                    this->_doc->GetElement((DB::ElementId ^)it->Current));
                p = v->Parameter[DB::BuiltInParameter::VIEWER_SHEET_NUMBER];
                if (p == nullptr || v->ViewType == DB::ViewType::Legend)
                    continue ;
                this->_namesIds->Add(String::Format("{0};{1};{2}",
                   v->Name,
                   v->Id->ToString(),
                   p->AsString() != "---" ? "1" : "0"),
                    v->Id);
            }
        }

        void    ViewsMgr::DeleteElements(CL::IList ^indices)
        {
            CL::IEnumerator ^it;
            DB::ElementId   ^id;
            DB::ElementId   ^cvId;

            it = indices->GetEnumerator();
            cvId = this->_doc->ActiveView->Id;
            while (it->MoveNext())
            {
                id = (DB::ElementId ^)this->_namesIds
                        ->GetByIndex((int)it->Current);
                if (cvId->Compare(id))
                    this->_doc->Delete(id);
            }
        }

        void    ViewsMgr::ViewsToViewData(GCL::ICollection<DB::ElementId ^> ^viewIds)
        {
            CL::IEnumerator     ^it;
            DB::View            ^view;
            ViewData            ^vd;            

            this->_viewData = gcnew CL::ArrayList(viewIds->Count);
            it = viewIds->GetEnumerator();
            while (it->MoveNext())
            {
                view = dynamic_cast<DB::View ^>(this->
                        _doc->GetElement((DB::ElementId ^)it->Current));
                if (view->IsTemplate)
                    continue ;
                vd = gcnew ViewData(view->Name, view->UniqueId,
                        view->ViewType, IS_ON_SHEET(view));
                this->_viewData->Add(vd);
            }
        }

        CL::ArrayList    ^ViewsMgr::GetViewData(void)
        {
            return (this->_viewData);
        }

        ViewData::ViewData(String ^name, String ^uniqueId,
                DB::ViewType viewType, char isOnSheet)
            : _name{name}, _uniqueId{uniqueId}, _viewType{viewType}, _isOnSheet(isOnSheet)
        {}
        String          ^ViewData::GetName(void)
        {
            return (this->_name);
        }
        String          ^ViewData::GetUniqueId(void)
        {
            return (this->_uniqueId);
        }
        DB::ViewType    ViewData::GetViewType(void)
        {
            return (this->_viewType);
        }
        char            ViewData::IsOnSheet(void)
        {
            return (this->_isOnSheet);
        }

        int     ViewData::CompareTo(System::Object ^obj)
        {
            ViewData    ^other;

            other = dynamic_cast<ViewData ^>(obj);
            if (other == nullptr)
                return (-1);
            return (String::Compare(this->GetName(), other->GetName()));
        }

        void    ViewsMgr::LogViewData(void)
        {
            System::Text::StringBuilder ^bldr;
            CL::IEnumerator             ^it;
            ViewData                    ^vd;

            it = this->GetViewData()->GetEnumerator();
            bldr = gcnew System::Text::StringBuilder();
            while (it->MoveNext())
            {
                vd = dynamic_cast<ViewData ^>(it->Current);
                bldr->AppendLine(String::Format("{0} {1} {2} {3}",
                    vd->GetName(), vd->GetUniqueId(), 
                    vd->GetViewType().ToString(), 
                    vd->IsOnSheet() == 1 ? "On Sheet" : "Not on sheet"));
            }
            LOG(bldr->ToString());
        }
    }
}