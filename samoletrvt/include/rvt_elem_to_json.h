// Copyright 2019 Samolet LLC
// Author: kkozlov
// rvt_elem_to_json.h

#ifndef RVT_ELEM_TO_JSON_H_
#define RVT_ELEM_TO_JSON_H_

namespace Samolet {
namespace Commands {
public ref class RvtElemConverter : JavaScriptConverter {
 public:
  RvtElemConverter(Autodesk::Revit::DB::Document ^doc);
  virtual property IEnumerableType^ SupportedTypes {
    IEnumerableType^ get(void) override;
  }
  virtual CLG::IDictionary<String^, Object^>^
  Serialize(Object ^obj, JavaScriptSerializer ^serializer) override;
  virtual Object^ Deserialize(CLG::IDictionary<String^, Object^> ^dic, Type ^type,
                              JavaScriptSerializer ^serializer) override;
 private:
  CLG::IDictionary<String^, Object^>^ ConvertParam(
      DB::Parameter ^p, CLG::IDictionary<String^,String^>^ name_id_dic);
  CLG::IDictionary<String^, Object^>^ ConvertLayers(DB::HostObjAttributes ^hoa);
  array<Object^>^ ConvertMaterials(DB::Element ^e);
  void SerializeRvtElem(DB::ElementId ^id, CL::ArrayList ^paramVals);

  DB::Document^	_doc;
  DateTime	_timestamp;
};

[Autodesk::Revit::Attributes::TransactionAttribute(
    Autodesk::Revit::Attributes::TransactionMode::Manual)]
[Autodesk::Revit::Attributes::RegenerationAttribute(
    Autodesk::Revit::Attributes::RegenerationOption::Manual)]
public ref class ParamsToJsonCmd : Autodesk::Revit::UI::IExternalCommand {
 public:
  virtual Autodesk::Revit::UI::Result Execute(
      Autodesk::Revit::UI::ExternalCommandData ^data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elems);
 private:
  Autodesk::Revit::UI::Result CarryOutCmd(
      Autodesk::Revit::UI::ExternalCommandData ^data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elems);
  int	HandleValidationFailure(Autodesk::Revit::DB::Document^ doc);
};
}  // namespace commands
}  // namespace samolet

#endif  // RVT_ELEM_TO_JSON_H_
