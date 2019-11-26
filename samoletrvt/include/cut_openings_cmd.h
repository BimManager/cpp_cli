// Copyright 2019 Samolet LLC
// Author: kkozlov
// cut_openings_cmd.h

#ifndef CUT_OPENINGS_CMD_H_
#define CUT_OPENINGS_CMD_H_

namespace Samolet {
namespace Commands {
interface class IClashResult;
ref class ProcessedClashResult;
ref class MEPElementCrossSection;

typedef System::Collections::Generic::SortedDictionary<
    Autodesk::Revit::DB::XYZ^, MEPElementCrossSection^>	SortedDictionaryXYZCrossSection;

enum OpeningType { RECTANGULAR, CIRCULAR };
  
[Autodesk::Revit::Attributes::TransactionAttribute(
    Autodesk::Revit::Attributes::TransactionMode::Manual)]
[Autodesk::Revit::Attributes::RegenerationAttribute(
    Autodesk::Revit::Attributes::RegenerationOption::Manual)]
[Autodesk::Revit::Attributes::JournalingAttribute(
    Autodesk::Revit::Attributes::JournalingMode::UsingCommandData)]
public ref class CutOpeningsCmd : UI::IExternalCommand {
public:
  virtual Autodesk::Revit::UI::Result Execute(
      Autodesk::Revit::UI::ExternalCommandData ^cmd_data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elements);
private:
  Autodesk::Revit::UI::Result	ExecuteCutOpeningsCmd(
      Autodesk::Revit::UI::ExternalCommandData ^cmd_data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elements);

  Autodesk::Revit::DB::FamilySymbol^	FindFamilySymbolAndCheckParameters(
      Autodesk::Revit::DB::Document^ doc, System::String^ family_name,
      System::String^ symbol_name,array<System::String^>^ param_names);

  System::Collections::Generic::ICollection<IClashResult^>^
      CarryOutClashDetection(ICollectionElement^ walls,
                             Autodesk::Revit::DB::Element^ link);

  IClashResult^	GetClashResult(
      Autodesk::Revit::DB::Wall^ wall, ICollectionElement^ mep_curves);
  
  Autodesk::Revit::DB::SolidCurveIntersection^	IntersectWallWithMEPCurve(
      Autodesk::Revit::DB::Wall^ wall, Autodesk::Revit::DB::MEPCurve^ mep_curve);

  void	InsertOpening(Autodesk::Revit::DB::Document^ doc,
                      Autodesk::Revit::DB::FamilySymbol^ fam_symbol,
                      ProcessedClashResult^ processed_data);

    //  ICollectionElement^	FindMEPCurvesInLinks(ICollectionElement^ links);
};

interface class IClashResult {
  Autodesk::Revit::DB::ElementId^	GetMainElementId(void);
  ICollectionElementId^			GetInterferingElementIds(void);
};

/*ref class ElementIdComparer : System::Collections::IEqualityComparer {
public:
  virtual bool	Equals(System::Object^ obj1, System::Object^ obj2);
  virtual int	GetHashCode(System::Object^ obj);
  };*/

ref class XYZComparer : System::Collections::IEqualityComparer {
public:
  virtual bool	Equals(System::Object^ obj1, System::Object^ obj2);
  virtual int	GetHashCode(System::Object^ obj);
};

ref class XYZSortedComparer :
    System::Collections::Generic::IComparer<Autodesk::Revit::DB::XYZ^> {
public:
  virtual int	Compare(Autodesk::Revit::DB::XYZ^ point_a,
                Autodesk::Revit::DB::XYZ^ point_b) {
    //if (point_a == point_b)
    if (Math::Round(point_a->X, 3) == Math::Round(point_b->X, 3) &&
        Math::Round(point_a->Y, 3) == Math::Round(point_b->Y, 3) &&
        Math::Round(point_a->Z, 3) == Math::Round(point_b->Z, 3))
      return (0);
    if (Math::Round(point_a->X, 3) == Math::Round(point_b->X, 3)) {
      if (Math::Round(point_a->Y, 3) > Math::Round(point_b->Y, 3))
        return (1);
      return (-1);
    }
    if (point_a->X > point_b->X)
      return (1);
    return (-1);
  }
};

ref class MEPElementCrossSection {
public:
  MEPElementCrossSection(Autodesk::Revit::DB::MEPCurve^ mep_crv) {
    try {
      _diameter = mep_crv->Diameter;
      _type = OpeningType::CIRCULAR;
      LOG(String::Format("diameter:{0}; type:{1}", _diameter, (int)_type));
    } catch (Autodesk::Revit::Exceptions::InvalidOperationException^ ex) {
      _height = mep_crv->Height;
      _width = mep_crv->Width;
      _type = OpeningType::RECTANGULAR;
      LOG(String::Format("height:{0}; width:{1}; type{2}",
                         _height, _width, (int)_type));
    }
  }
  OpeningType	GetOpeningType(void) {
    return (_type);
  }
  double	GetDiameter(void) {
    return (_diameter);
  }
  double	GetHeight(void) {
    return (_height);
  }
  double	GetWidth(void) {
    return (_width);
  }
  double	GetMaxDimension(void) {
    if (_type == OpeningType::CIRCULAR)
      return (_diameter);
    else if (_width > _height)
      return (_width);
    return (_height);
  }
private:
  OpeningType	_type;
  double	_diameter;
  double	_height;
  double	_width;
};

ref class WallMEPCurvesClashResult : IClashResult {
public:
  WallMEPCurvesClashResult(Autodesk::Revit::DB::ElementId^ main_id);
  
  virtual	Autodesk::Revit::DB::ElementId^	GetMainElementId(void);
  virtual	ICollectionElementId^		GetInterferingElementIds(void);

  void		AddPointId(Autodesk::Revit::DB::XYZ^ clash_point,
                           Autodesk::Revit::DB::ElementId^ elem_id);

  void		AddPointCrossSection(Autodesk::Revit::DB::XYZ^ clash_point,
                                     Autodesk::Revit::DB::MEPCurve^ mep_crv);
  
  ICollectionXYZ^	GetClashPoints(void);
  double		GetMaxDimension(void) {
    LOG("max_dimension: " + _max_dimension);
    return (_max_dimension);
  }
  MEPElementCrossSection^	GetCrossSection(Autodesk::Revit::DB::XYZ^ point) {
    LOG("Extracting point: " + point->ToString());
    return (_points_sections[point]);
  }
private:
  Autodesk::Revit::DB::ElementId^	_main_id;
  SortedDictionaryXYZElementId^		_points_ids;
  SortedDictionaryXYZCrossSection^	_points_sections;
  double				_max_dimension;
};

ref class ClashResultProcessor {
public:
  ClashResultProcessor(void);
  
  System::Collections::Generic::ICollection<ProcessedClashResult^>^
      Process(IClashResult^ result);
  ICollectionXYZ^	GetClusterCentroids(void);
  //  ProcessedClashResult^	GetProcessedClashResult(void);
private:
  ICollectionXYZ^	DetectCluster(ICollectionXYZ^ points, double radius);  
  System::Collections::Generic::LinkedList<ICollectionXYZ^>^
      DBSCAN(ICollectionXYZ^ points, double radius);
  Autodesk::Revit::DB::XYZ^	ComputeClusterCentroid(ICollectionXYZ^ cluster);
  array<Autodesk::Revit::DB::XYZ^>^	LocateMinMaxPointsInCluster(ICollectionXYZ^ cluster);
  ProcessedClashResult^			ProcessCluster(WallMEPCurvesClashResult^ clash_result,
                                                       ICollectionXYZ^ cluster);

  LinkedListXYZ^			centroids_;
  Autodesk::Revit::DB::Document^	_doc;
};

ref class ProcessedClashResult {
public:
  ProcessedClashResult(Autodesk::Revit::DB::XYZ^ centroid,
                       array<Autodesk::Revit::DB::XYZ^>^ min_max,
                       WallMEPCurvesClashResult^ clash_result) {
    _insertion_point = centroid;
    _host_id = clash_result->GetMainElementId();
    LOG("min: " + min_max[0]->ToString());
    LOG("max: " + min_max[1]->ToString());
    ComputeDimensions(min_max, clash_result->GetCrossSection(min_max[0]),
                      clash_result->GetCrossSection(min_max[1]));
  }
  Autodesk::Revit::DB::XYZ^	GetInsertionPoint(void) {
    return (_insertion_point);
  }
  Autodesk::Revit::DB::ElementId^	GetHostId(void) {
    return (_host_id);
  }
  OpeningType			GetOpeningType(void) {
    return (_opening_type);
  }
  double			GetRadius(void) {
    return (_radius);
  }
  double			GetHeight(void) {
    return (_height);
  }
  double			GetWidth(void) {
    return (_width);
  }
private:
  void				ComputeDimensions(array<Autodesk::Revit::DB::XYZ^>^ min_max,
                                                  MEPElementCrossSection^ section_1,
                                                  MEPElementCrossSection^ section_2);
  Autodesk::Revit::DB::XYZ^	_insertion_point;
  Autodesk::Revit::DB::ElementId^	_host_id;
  OpeningType			_opening_type;
  double			_radius;
  double			_height;
  double			_width;
};

}  // namespace Commands
}  // namespace Samolet


#endif  // CUT_OPENINGS_CMD_H_
