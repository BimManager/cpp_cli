// Copyright 2019 Samolet LLC
// Author: kkozlov
// cut_openings_cmd.cpp

#include "cut_openings_cmd.h"

#include "utils.h"

namespace Samolet {
namespace Commands {
namespace UI = Autodesk::Revit::UI;
namespace DB = Autodesk::Revit::DB;

UI::Result CutOpeningsCmd::Execute(UI::ExternalCommandData^ cmd_data,
                           String^% msg, DB::ElementSet^ elements) {
  return (ExecuteCutOpeningsCmd(cmd_data, msg, elements));
}

// 1. Find the family symbol and check the presence of the parameters
// 2. Collect all the wall in the active document
// 3. Prompt the user to pick out an MEP document
// 4. Carry out clash detection
// (For each wall find all MEP objects that interfere with it.)
// 5. Process the results of the clash detection
// 6. Insert the family instance at the intersection point
// 7. Update the parameters of the family instance
UI::Result CutOpeningsCmd::ExecuteCutOpeningsCmd(
    UI::ExternalCommandData^ cmd_data, String^% msg, DB::ElementSet^ elements) {
  UI::UIDocument^	uidoc;
  DB::Document^		doc;
  ICollectionElement^	walls;
  ICollectionElement^	links;
  DB::Reference^	ref_link_doc;
  CLG::ICollection<IClashResult^>^	results;
  CLG::IEnumerator<IClashResult^>^	it_res;
  CLG::IEnumerator<DB::XYZ^>^		it_xyz;
  DB::Transaction^			tx;
  DB::FamilySymbol^			family_symbol;
  DB::FamilyInstance^			family_inst;
  ClashResultProcessor^			processor;
  DB::Parameter^			param;
  
  uidoc = cmd_data->Application->ActiveUIDocument;
  doc = uidoc->Document;
  // 1.
  if ((family_symbol = FindFamilySymbolAndCheckParameters(
          doc,"opening_rectangular", "foo", nullptr)) == nullptr)
    return UI::Result::Failed;
  // 2.
  walls = Utils::RvtFilters::FindElemsOfClass(doc, DB::Wall::typeid);
  LOG(String::Format("walls found: {0}", walls->Count));
  // 3.
  ref_link_doc = uidoc->Selection->PickObject(
      SEL::ObjectType::Element, "Please pick out an MEP link.");
  // 4.
  results = CarryOutClashDetection(walls, doc->GetElement(ref_link_doc));
  
  it_res = results->GetEnumerator();
  tx = gcnew DB::Transaction(doc);
  processor = gcnew ClashResultProcessor();
  tx->Start("Insert openings.");
  IEnumerator^	fres_it;
  while (it_res->MoveNext()) {
    // 5.
    fres_it = processor->Process(it_res->Current)->GetEnumerator();
    while (fres_it->MoveNext())
      InsertOpening(doc, family_symbol,
                    static_cast<ProcessedClashResult^>(fres_it->Current));
  }
  tx->Commit();
  return (UI::Result::Succeeded);
}

DB::FamilySymbol^	CutOpeningsCmd::FindFamilySymbolAndCheckParameters(
    DB::Document^ doc, String^ family_name, String^ symbol_name,
    array<String^>^ param_names) {
  DB::FamilySymbol^	family_symbol;
  
  if ((family_symbol = Utils::RvtFilters::FindFamilySymbolByName(
          doc,"opening_rectangular", "foo")) != nullptr) {
    LOG(String::Format("Family Type: {0} has been found.",
                       family_symbol->Name));
    return (family_symbol);
  }
  return (nullptr);
}

CLG::ICollection<IClashResult^>^ CutOpeningsCmd::CarryOutClashDetection(
    ICollectionElement^ walls, Autodesk::Revit::DB::Element^ link) {
  IEnumeratorElement^			wall_it;
  DB::Document^				mep_doc;
  ICollectionElement^			clash_elems;
  CLG::LinkedList<IClashResult^>^	results;

  results = gcnew CLG::LinkedList<IClashResult^>();
  wall_it = walls->GetEnumerator();
  mep_doc = static_cast<DB::RevitLinkInstance^>(link)->GetLinkDocument();
  LOG(mep_doc->Title);
  while (wall_it->MoveNext()) {
    clash_elems = Utils::RvtFilters::FindMEPCurvesIntersectingWall(
        mep_doc, static_cast<DB::Wall^>(wall_it->Current));
    if (clash_elems->Count) {
      LOG(String::Format("wall {0} is intersected by {1} elements.",
                         wall_it->Current->Id, clash_elems->Count));
      results->AddFirst(GetClashResult(
          static_cast<DB::Wall^>(wall_it->Current), clash_elems));
    }
  }
  return (results);
}

/*ICollectionElement^ CutOpeningsCmd::FindMEPCurvesInLinks(
    ICollectionElement^ links) {
  LinkedListElement^		mep_curves;
  IEnumeratorElement^		it;
  IEnumeratorElement^		crv_it;
  DB::RevitLinkInstance^	link;
  DB::Document^			link_doc;
  ICollectionElement^		curves;

  mep_curves = gcnew LinkedListElement();
  it = links->GetEnumerator();
  while (it->MoveNext()) {
    link = static_cast<DB::RevitLinkInstance^>(it->Current);
    if ((link_doc = link->GetLinkDocument()) != nullptr) {
      curves = Utils::RvtFilters::FindElemsOfClass(link_doc,
                                                   DB::MEPCurve::typeid);
      crv_it = curves->GetEnumerator();
      while(crv_it->MoveNext())
        mep_curves->AddFirst(crv_it->Current);
      LOG(String::Format("link: {0}; curves: {1}",
                         link_doc->Title, curves->Count));
    }
  }
  return (mep_curves);
  }*/

IClashResult^	CutOpeningsCmd::GetClashResult(
    Autodesk::Revit::DB::Wall^ wall, ICollectionElement^ clash_elems) {
  IEnumeratorElement^		elem_it;
  DB::SolidCurveIntersection^	clash_crv;
  WallMEPCurvesClashResult^	clash_result;
  DB::Curve^			crv;			

  elem_it = clash_elems->GetEnumerator();
  clash_result = gcnew WallMEPCurvesClashResult(wall->Id);
  LOG("clash elements: " + clash_elems->Count);
  while (elem_it->MoveNext()) {
    if ((clash_crv = IntersectWallWithMEPCurve(
            wall, static_cast<DB::MEPCurve^>(elem_it->Current))) != nullptr) {
      crv = clash_crv->GetCurveSegment(0);
      clash_result->AddPointId((crv->GetEndPoint(0) + crv->GetEndPoint(1)) / 2,
                               elem_it->Current->Id);
      LOG(String::Format("wall {0} is intersected at {1}.",
                         wall->Id->IntegerValue,
                         (crv->GetEndPoint(0) + crv->GetEndPoint(1)) / 2));
      clash_result->AddPointCrossSection((crv->GetEndPoint(0) + crv->GetEndPoint(1)) / 2,
                                         static_cast<DB::MEPCurve^>(elem_it->Current));
    }
  }
  return (clash_result);
}

DB::SolidCurveIntersection^ CutOpeningsCmd::IntersectWallWithMEPCurve(
    DB::Wall^ wall, DB::MEPCurve^ mep_curve) {
  DB::Solid^			wall_solid;
  DB::LocationCurve^		location_curve;
  DB::SolidCurveIntersection^	crv;

  if ((wall_solid = Utils::RvtGeometryHandler::GetSolid(wall)) != nullptr
      && (location_curve = dynamic_cast<DB::LocationCurve^>(
          mep_curve->Location)) != nullptr) {
    crv = wall_solid->IntersectWithCurve(
        location_curve->Curve, gcnew DB::SolidCurveIntersectionOptions());
    if (crv->SegmentCount > 0)
      return (crv);
  }
  return (nullptr);
}

void CutOpeningsCmd::InsertOpening(
    DB::Document^ doc, DB::FamilySymbol^ fam_symbol,
    ProcessedClashResult^ processed_data) {
  DB::FamilyInstance^	fam_inst;
  DB::Parameter^	height_param;
  DB::Parameter^	width_param;

  LOG("========================");
  LOG(String::Format("Point:{0}; HostId:{1}; H{2}xW{3}",
                     processed_data->GetInsertionPoint()->ToString(),
                     processed_data->GetHostId()->IntegerValue,
                     processed_data->GetHeight(),
                     processed_data->GetWidth()));
  LOG("========================");
  fam_inst = doc->Create->NewFamilyInstance(
      processed_data->GetInsertionPoint(),
      fam_symbol,
      doc->GetElement(processed_data->GetHostId()),
      static_cast<DB::Level^>(doc->GetElement(
          doc->GetElement(processed_data->GetHostId())->LevelId)),
      DB::Structure::StructuralType::NonStructural);
  doc->Regenerate();
  if ((height_param = fam_inst->LookupParameter("opening_height")) != nullptr
      && (width_param = fam_inst->LookupParameter("opening_width")) != nullptr) {
    height_param->Set(processed_data->GetHeight());
    width_param->Set(processed_data->GetWidth());
    LOG("The parameter has been set.");
    doc->Regenerate();
    DB::XYZ^	new_location = gcnew
        DB::XYZ(0, 0, - processed_data->GetHeight() / 2);
    DB::ElementTransformUtils::MoveElement(
        doc, fam_inst->Id, new_location);
  }
}

WallMEPCurvesClashResult::WallMEPCurvesClashResult(DB::ElementId^ mainid) 
    : _main_id{mainid}, _max_dimension{0} {
      _points_ids = gcnew SortedDictionaryXYZElementId(
          gcnew XYZSortedComparer());
      _points_sections = gcnew SortedDictionaryXYZCrossSection(
          gcnew XYZSortedComparer());
}

void WallMEPCurvesClashResult::AddPointId(
    DB::XYZ^ clash_point, DB::ElementId^ elem_id) {
    if (!_points_ids->ContainsKey(clash_point)) 
      _points_ids->Add(clash_point, elem_id);
}

void WallMEPCurvesClashResult::AddPointCrossSection(DB::XYZ^ clash_point,
                                                    DB::MEPCurve^ mep_crv) {
  MEPElementCrossSection^	section;

  if (!_points_sections->ContainsKey(clash_point)) {
    section = gcnew MEPElementCrossSection(mep_crv);
    _points_sections->Add(clash_point, section);
    LOG("point added: " + clash_point->ToString());
    if (_max_dimension < section->GetMaxDimension())
      _max_dimension = section->GetMaxDimension();
  }
}

DB::ElementId^ WallMEPCurvesClashResult::GetMainElementId(void) {
  return (_main_id);
}

ICollectionElementId^ WallMEPCurvesClashResult::GetInterferingElementIds(void) {
  return (_points_ids->Values);
}

ICollectionXYZ^ WallMEPCurvesClashResult::GetClashPoints(void) {
  return (_points_ids->Keys);
}

/*bool ElementIdComparer::Equals(Object^ obj1, Object^ obj2) {
  DB::ElementId^	id1;
  DB::ElementId^	id2;

  if ((id1 = dynamic_cast<DB::ElementId^>(obj1)) != nullptr &&
      (id2 = dynamic_cast<DB::ElementId^>(obj2)) != nullptr) {
    return (id1 == id2);
  }
  return (false);
}

int ElementIdComparer::GetHashCode(Object^ obj) {
  return (obj->GetHashCode());
  }*/


bool XYZComparer::Equals(Object^ obj1, Object^ obj2) {
  DB::XYZ^	pnt1;
  DB::XYZ^	pnt2;

  if ((pnt1 = dynamic_cast<DB::XYZ^>(obj1)) != nullptr &&
      (pnt2 = dynamic_cast<DB::XYZ^>(obj2)) != nullptr)
    return (pnt1 == pnt2);
  return (false);
}

int XYZComparer::GetHashCode(Object^ obj) {
  return (obj->GetHashCode());
}

ClashResultProcessor::ClashResultProcessor(void) {
  centroids_ = gcnew LinkedListXYZ();
}

CLG::ICollection<ProcessedClashResult^>^ ClashResultProcessor::Process(
    IClashResult^ result) {
  CLG::LinkedList<ProcessedClashResult^>^	results;
  IEnumerator^					cluster_it;
  WallMEPCurvesClashResult^			clash_result;
  
  centroids_->Clear();
  clash_result = static_cast<WallMEPCurvesClashResult^>(result);
  cluster_it = DBSCAN(clash_result->GetClashPoints(),
                      clash_result->GetMaxDimension() * 4 /* multiplier */
                      )->GetEnumerator();
  results = gcnew CLG::LinkedList<ProcessedClashResult^>();
  while (cluster_it->MoveNext())
    results->AddLast(ProcessCluster(clash_result,
        static_cast<ICollectionXYZ^>(cluster_it->Current)));
  return (results);
}

ProcessedClashResult^ ClashResultProcessor::ProcessCluster(
    WallMEPCurvesClashResult^ clash_result, ICollectionXYZ^ cluster) {
  ProcessedClashResult^	processed_result;

  processed_result = gcnew ProcessedClashResult(
      ComputeClusterCentroid(cluster),
      LocateMinMaxPointsInCluster(cluster),
      clash_result);
  return (processed_result);
}
    

ICollectionXYZ^	ClashResultProcessor::GetClusterCentroids(void) {
  return (centroids_);
}

ICollectionXYZ^ ClashResultProcessor::DetectCluster(
    ICollectionXYZ^ points, double radius) {
  CL::IEqualityComparer^	comparer;
  CL::Queue^			points_to_check;
  CL::Hashtable^		checked_points;
  LinkedListXYZ^		cluster;
  DB::XYZ^			cluster_point;
  DB::XYZ^			point;
  IEnumerator^			xyz_it;

  comparer = gcnew XYZComparer();
  LOG("points: " + points->Count);
  points_to_check = gcnew CL::Queue(points->Count);
  cluster = gcnew LinkedListXYZ();
  checked_points = gcnew CL::Hashtable(comparer);
  xyz_it = points->GetEnumerator();
  xyz_it->MoveNext();
  points_to_check->Enqueue(xyz_it->Current);
  while (points_to_check->Count) {
    LOG("Point to check: " + points_to_check->Peek()->ToString());
    cluster_point = static_cast<DB::XYZ^>(points_to_check->Dequeue());
    xyz_it->Reset();
    while (xyz_it->MoveNext()) {
      point = static_cast<DB::XYZ^>(xyz_it->Current);
      if (!checked_points->Contains(point) &&
          cluster_point->DistanceTo(point) <= radius) {
        cluster->AddFirst(point);
        points_to_check->Enqueue(point);
        checked_points->Add(point, nullptr);
      }
    }
  }
#ifdef DEBUG
  xyz_it = cluster->GetEnumerator();
  LOG("====Start=====");
  while (xyz_it->MoveNext())
    LOG(xyz_it->Current->ToString());
  LOG("=====End=====");
#endif
  return (cluster);
}

CLG::LinkedList<ICollectionXYZ^>^	ClashResultProcessor::DBSCAN(
    ICollectionXYZ^ points, double radius) {
  CLG::LinkedList<ICollectionXYZ^>^	clusters;
  LinkedListXYZ^			unprocessed_points;
  ICollectionXYZ^			cluster;
  IEnumerator^				it;

  unprocessed_points = gcnew LinkedListXYZ(points);
  clusters = gcnew CLG::LinkedList<ICollectionXYZ^>();
  while (unprocessed_points->Count) {
    cluster = DetectCluster(unprocessed_points, radius);
    centroids_->AddLast(ComputeClusterCentroid(cluster));
    clusters->AddLast(cluster);
    it = cluster->GetEnumerator();
    while (it->MoveNext())
      unprocessed_points->Remove(static_cast<DB::XYZ^>(it->Current));
  }
  return (clusters);
}

DB::XYZ^ ClashResultProcessor::ComputeClusterCentroid(ICollectionXYZ^ cluster) {
  IEnumerator^	it;
  int		n_points;
  DB::XYZ^	centroid;

  n_points = cluster->Count;
  it = cluster->GetEnumerator();
  centroid = gcnew DB::XYZ(0, 0, 0);
  while (it->MoveNext())
    centroid += static_cast<DB::XYZ^>(it->Current);
  centroid /= n_points;
  LOG("cluster's centroid: " + centroid->ToString());
  return (centroid);
}

array<DB::XYZ^>^ ClashResultProcessor::LocateMinMaxPointsInCluster(
    ICollectionXYZ^ cluster) {
  DB::XYZ^		cur;
  DB::XYZ^		min;
  DB::XYZ^		max;
  XYZSortedComparer^	cmp;
  IEnumerator^		xyz_it;
  int			res;
  array<DB::XYZ^>^	min_max;

  min_max = gcnew array<DB::XYZ^>(2);
  xyz_it = cluster->GetEnumerator();
  xyz_it->MoveNext();
  min = static_cast<DB::XYZ^>(xyz_it->Current);
  max = static_cast<DB::XYZ^>(xyz_it->Current);
  xyz_it->Reset();
  cmp = gcnew XYZSortedComparer();
  while (xyz_it->MoveNext()) {
    cur = static_cast<DB::XYZ^>(xyz_it->Current);
    LOG(String::Format("min: {0}; max:{1}; cur:{2}",
                       min->ToString(), max->ToString(),
                       cur->ToString()));
    res = cmp->Compare(cur, min);
    if (res < 0)
      min = cur;
    res = cmp->Compare(cur, max);
    if (res > 0)
      max = cur;
  }
  LOG("cluster's min point: " + min->ToString());
  LOG("cluster's max point: " + max->ToString());
  min_max[0] = min;
  min_max[1] = max;
  return (min_max);
}

void ProcessedClashResult::ComputeDimensions(array<Autodesk::Revit::DB::XYZ^>^ min_max,
                                             MEPElementCrossSection^ section_1,
                                             MEPElementCrossSection^ section_2) {
  double	distance;
  double	delta_z;
  double	cos_alpha;

  if (min_max[0] != min_max[1]) {
    distance = min_max[0]->DistanceTo(min_max[1]);
    delta_z = Math::Round(Math::Abs(min_max[1]->Z) - Math::Abs(min_max[0]->Z), 3);
    cos_alpha = Math::Sqrt(1 - Math::Pow(delta_z / distance, 2));
    LOG("cos_alpha: " + cos_alpha);
    LOG("delta_z: " + delta_z);
    LOG("distance " + distance);
    _width = distance * cos_alpha;
    _height = delta_z;
    _width += section_1->GetMaxDimension();
    _width += section_2->GetMaxDimension();
    _height += section_1->GetMaxDimension();
    _height += section_2->GetMaxDimension();
    return ;
  }
  _height = section_1->GetMaxDimension() * 2;
  _width = section_1->GetMaxDimension() * 2;
}

}  // namespace Commands
}  // namespace Samolet
