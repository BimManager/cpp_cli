// Copyright 2019 Samolet LLC
// Author: kkozlov
// utils.h

#ifndef UTILS_H_
# define UTILS_H_

# define SUCCESS		0
# define CFM_KEY_DOES_NOT_EXIST	1
# define CFM_FILE_DOES_NOT_EXIT	2

namespace Samolet {  
namespace Utils {
public ref class ResourceManager {
 public:
  static System::IO::MemoryStream^ GetMemoryStream(
      System::Reflection::Assembly ^asmb, System::String ^rsName,
      System::String ^imgName);
  static IMG::BitmapImage^ MemoryStreamToImage(System::IO::MemoryStream ^ms);
};

public ref class ConfigFileManager {
 public:
  static int GetValueFromConfigFile(System::String ^key, System::String ^%out);
  static int GetFilePath(System::String ^key, System::String ^%outFilePath);
  System::String^ GetConnectionStringByName(System::String^ name);
};

public ref class FileUtils {
 public:
  static System::String^ CreateTempFile(void);
  static void SaveToFile(System::String^ filepath, System::String^ data);
  static array<String^>^ ReadNLines(System::String^ filepath, int n_lines);
};

public ref class HandyUtils {
 public:
  static array<System::String^>^ StringifyEnumValues(System::Type^ enum_type);
  static System::Security::SecureString^ StringToSecureString(System::String^ string);
  static System::String^ SecureStringToString(System::Security::SecureString^ secure_string);
};
 
public ref class RvtParamManager {
 public:
  static void SetStringValue(Autodesk::Revit::DB::Parameter ^param,
                             System::String ^valAsStr);
  static System::String^ GetValueAsString(Autodesk::Revit::DB::Parameter ^param);
  static System::String^ GetValueAsString(Autodesk::Revit::DB::Document ^doc,
                                          Autodesk::Revit::DB::Parameter ^param);
 private:
  static System::String^	GetElemName(Autodesk::Revit::DB::Element ^elem);
};

public ref class RvtFilters {
public:
  static ICollectionElement^ FindElemsOfClassByTypeId(
      Autodesk::Revit::DB::Document ^doc, System::Type ^type,
      Autodesk::Revit::DB::ElementId ^type_id);
  static ICollectionElementId^ FindAllQuantifiableElemIds(
      Autodesk::Revit::DB::Document ^doc);
  static ES::DataStorage^ FindDataStorage(Autodesk::Revit::DB::Document^ doc,
                                          System::String^ name);
  static ICollectionElement^ FindRevitLinkInstances(
      Autodesk::Revit::DB::Document^ doc);
  static ICollectionElement^ FindElemsOfClass(
      Autodesk::Revit::DB::Document^ doc, System::Type^ type);
  static ICollectionElement^ FindMEPCurvesIntersectingWall(
      Autodesk::Revit::DB::Document^ doc, Autodesk::Revit::DB::Wall^ wall);
  static Autodesk::Revit::DB::FamilySymbol^ FindFamilySymbolByName(
      Autodesk::Revit::DB::Document^ doc, System::String^ family_name,
      System::String^ symbol_name);
  static ICollectionCategory^ FindCategoriesByBuiltInName(
      Autodesk::Revit::DB::Document^ doc,
      System::Collections::IEnumerable^ names);
  static ICollectionElementId^ FindAllBOQElemIds(
      Autodesk::Revit::DB::Document^ doc);
      
private:
  static ICollectionCategory^ FindAllQtyCategories(
      Autodesk::Revit::DB::Document^ doc);
  static ICollectionElementId^ FindAllElementsByCategory(
      Autodesk::Revit::DB::Document^ doc,
      ICollectionCategory^ cats);
};

public ref class RvtGeometryHandler {
 public:
  static Autodesk::Revit::DB::Solid^ GetSolid(Autodesk::Revit::DB::Element^ elem);
  static Autodesk::Revit::DB::XYZ^ GetCentroid(Autodesk::Revit::DB::Element^ elem);
 private:
  static Autodesk::Revit::DB::Solid^ GetSolid(
      Autodesk::Revit::DB::GeometryInstance^ geom_inst);
};

public ref class ViewsManager {
public:
  static void ResetOverrideGraphicSettings(Autodesk::Revit::DB::Document ^doc,
                                           Autodesk::Revit::DB::ElementId ^eid);
}; 
}  // namespace Utils
}  // namespace Samolet
#endif  // UTILS_H_
