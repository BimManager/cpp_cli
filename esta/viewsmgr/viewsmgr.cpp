/*
 *  rvt_tmpl.cpp
 */

#include "viewsmgr.h"
//#include "viewsmgr_form.h"
#include "viewsmgrForm.h"

using namespace Esta;
using namespace Esta::ViewsMgnt;
using namespace Esta::Gui;
using namespace Commands;

Autodesk::Revit::UI::Result ViewsMgrCmd::Execute(
    Autodesk::Revit::UI::ExternalCommandData ^cmdData,
    System::String ^%msg,
    Autodesk::Revit::DB::ElementSet ^elements)
{
    ViewsMgr        ^mgr;
    Transaction ^tr;
    Document    ^doc;
    MgrForm         ^form;

    doc = cmdData->Application->ActiveUIDocument->Document;
    mgr = gcnew ViewsMgr(doc);
    mgr->Execute();

    form = gcnew MgrForm(mgr->GetViewData());   
    form->ShowDialog();
    tr = gcnew Transaction(doc);
    tr->Start("Delete views");
    mgr->DeleteViews(form->GetSelectedIds());
    tr->Commit();
    return (Autodesk::Revit::UI::Result::Succeeded);
}

ViewsMgr::ViewsMgr(DB::Document ^doc)
{
    this->_doc = doc;
}

SortedList  ^ViewsMgr::GetNamesIds(void)
{
    return (this->_namesIds);
}

void    ViewsMgr::Execute(void)
{
    ICollection<Element ^>     ^views;

    views = this->CollectAllViews();
    this->ViewsToViewData(views);
}

GCL::ICollection<Element ^>    ^ViewsMgr::CollectAllViews(void)
{
    FilteredElementCollector   ^col;
    ElementCategoryFilter      ^viewCatFltr;
    ICollection<Element ^>     ^views;

    col = gcnew FilteredElementCollector(this->_doc);
    viewCatFltr = gcnew ElementCategoryFilter(DB::BuiltInCategory::OST_Views);
    views = col->WherePasses(viewCatFltr)
                ->WhereElementIsNotElementType()
                ->ToElements();
    return (views);
}

void    ViewsMgr::DeleteViews(Hashtable ^uniqueIds)
{
    CL::IEnumerator ^it;
    DB::Document    ^doc;
    DB::Element     ^elem;

    doc = this->_doc;
    if (uniqueIds->ContainsKey(doc->ActiveView->UniqueId))
        uniqueIds->Remove(doc->ActiveView->UniqueId);
    it = uniqueIds->Keys->GetEnumerator();
    while (it->MoveNext())
        if ((elem = doc->GetElement(static_cast<String ^>(it->Current))) != nullptr)
            doc->Delete(elem->Id);
}

void    ViewsMgr::ViewsToViewData(GCL::ICollection<Element ^> ^views)
{
    IEnumerator     ^it;
    View            ^view;
    ViewData        ^vd;            

    this->_viewData = gcnew ArrayList(views->Count);
    it = views->GetEnumerator();
    while (it->MoveNext())
    {
        view = dynamic_cast<DB::View ^>(it->Current);
        if (view->IsTemplate)
            continue ;
        vd = gcnew ViewData(view->Name, view->UniqueId,
                view->ViewType, IS_ON_SHEET(view));
        this->_viewData->Add(vd);
    }
    this->_viewData->Sort();
}

ArrayList    ^ViewsMgr::GetViewData(void)
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
    //return (String::Compare(this->GetName(), other->GetName()));
    return (String::Compare(
        this->GetViewType().ToString(), other->GetViewType().ToString()));
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
#if DEBUG
    System::Debug::Write(bldr->ToString());
#endif          
}
