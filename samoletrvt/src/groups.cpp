// Copyright 2019 Samolet LLC
// Author: kkozlov
// groups.h

#include "groups.h"

#include "utils.h"

namespace Samolet {
namespace Helpers {
GroupHandler::GroupHandler(DB::Document ^doc)
    : doc_{doc} {}

ICollectionElement^ GroupHandler::Execute(DB::Element ^elem) {
  ICollectionElement^	groups;
  DB::Group^		group;
  LinkedListElement^	ret;
  IEnumerator^		group_it;
  DB::Element^		found_elem;

  ret = gcnew CLG::LinkedList<DB::Element^>();
  if (elem->GroupId == DB::ElementId::InvalidElementId)
    return (ret);
  group = static_cast<DB::Group^>(doc_->GetElement(elem->GroupId));
  if (ComputeDelta(elem, group))
      return (ret);
  groups = Utils::RvtFilters::FindElemsOfClassByTypeId(
      doc_, DB::Group::typeid, group->GetTypeId());
  group_it = groups->GetEnumerator();
  while (group_it->MoveNext()) {
    found_elem = FindElemInGroup(static_cast<DB::Group^>(group_it->Current),
                           elem->Category->Id);
    if (elem->Id->IntegerValue != found_elem->Id->IntegerValue)
      ret->AddFirst(found_elem);
  }
  return (ret);
}

int GroupHandler::ComputeDelta(DB::Element ^elem,
                                DB::Group ^group) {
  DB::LocationPoint^	elem_pnt;
  DB::XYZ^		group_pnt;
  DB::XYZ^		centroid;
  int			ret;

  ret = 0;
  group_pnt = static_cast<DB::LocationPoint^>(group->Location)->Point;
  if ((elem_pnt = dynamic_cast<DB::LocationPoint^>(elem->Location)) != nullptr)
    delta_ = elem_pnt->Point - group_pnt;
  else if ((centroid = Utils::RvtGeometryHandler::GetCentroid(elem)) != nullptr)
    delta_ = centroid - group_pnt;
  else if ((centroid = GetPointFromRebar(elem)) != nullptr) {
    LOG("Rebar centroid: " + centroid->ToString());
    delta_ = centroid - group_pnt;
  }
  else
    ret = 1;
  return (ret);
}

DB::Element^ GroupHandler::FindElemInGroup(DB::Group ^group,
                                          DB::ElementId ^catId){
  DB::FilteredElementCollector^		col;
  DB::BoundingBoxContainsPointFilter^	bbox_contains_pnt_fltr;
  DB::XYZ^				group_pnt;
  
  col = gcnew DB::FilteredElementCollector(doc_, group->GetMemberIds());
  group_pnt = static_cast<DB::LocationPoint^>(group->Location)->Point;
  bbox_contains_pnt_fltr =
      gcnew DB::BoundingBoxContainsPointFilter(group_pnt + delta_);
  return (col->OfCategoryId(catId)
             ->WherePasses(bbox_contains_pnt_fltr)
             ->FirstElement());
}

DB::XYZ^ GroupHandler::GetPointFromRebarGeometry(DB::Element^ elem) {
  DB::Options^	options;
  IEnumerator^	geom_obj_it;
  DB::Curve^	crv;

  options = gcnew DB::Options();
  options->ComputeReferences = true;
  options->DetailLevel = DB::ViewDetailLevel::Fine;
  options->IncludeNonVisibleObjects = true;
  geom_obj_it = elem->Geometry[options]->GetEnumerator();
  while (geom_obj_it->MoveNext())
    if ((crv = dynamic_cast<DB::Curve^>(geom_obj_it->Current)) != nullptr)
      return ((crv->GetEndPoint(0) + crv->GetEndPoint(1)) / 2);
  return (nullptr);
}

DB::XYZ^ GroupHandler::GetPointFromRebar(DB::Element^ elem) {
  DB::Structure::Rebar^		rebar;
  DB::Structure::RebarInSystem^	rebar_system;

  if ((rebar = dynamic_cast<DB::Structure::Rebar^>(elem)) != nullptr)
    return (GetPointFromRebarGeometry(rebar));
  else if ((rebar_system = dynamic_cast<
            DB::Structure::RebarInSystem^>(elem)) != nullptr)
    return (GetPointFromRebarGeometry(rebar_system));
  return (nullptr);
}

}  // namespace Helpers
}  // namespace Samolet

