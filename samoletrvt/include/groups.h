// Copyright 2019 Samolet LLC
// Author: kkozlov
// GroupHandler.h

#ifndef GROUP_HANDLER_H
#define GROUP_HANDLER_H

#include "stdafx.h"

namespace Samolet {
namespace Helpers {
public ref class GroupHandler {
public:
  GroupHandler(Autodesk::Revit::DB::Document^ doc);
  
  ICollectionElement^ Execute(Autodesk::Revit::DB::Element^ elem);
private:
  Autodesk::Revit::DB::XYZ^ GetPointFromRebar(Autodesk::Revit::DB::Element^ e);
  int ComputeDelta(Autodesk::Revit::DB::Element ^elem,
                   Autodesk::Revit::DB::Group ^group);
  Autodesk::Revit::DB::Element^ FindElemInGroup(
      Autodesk::Revit::DB::Group ^group, Autodesk::Revit::DB::ElementId ^catId);
  Autodesk::Revit::DB::XYZ^ GetPointFromRebarGeometry(
      Autodesk::Revit::DB::Element^ elem);
  
  Autodesk::Revit::DB::Document^	doc_;
  Autodesk::Revit::DB::XYZ^		delta_;
};
} // namespace Helpers
} // namespace Samolet

#endif
