// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_values_cmd.h

#ifndef SELECT_VALUES_CMD_H_
# define SELECT_VALUES_CMD_H_

# include "set_of_values.h"
# include "params_data_parser.h"
# include "select_values_form.h"
# include "failures.h"
# include "web.h"
# include "utils.h"
# include "groups.h"

namespace Samolet {
namespace Commands {
public ref class AvailableUponSelection :
Autodesk::Revit::UI::IExternalCommandAvailability {
 public:
  virtual bool IsCommandAvailable(
      Autodesk::Revit::UI::UIApplication ^uiapp,
      Autodesk::Revit::DB::CategorySet ^selCats);
};

ref class ParameterEqualityCompare :
System::Collections::Generic::IEqualityComparer<
  Autodesk::Revit::DB::Parameter^> {
 public:
  virtual bool Equals(Autodesk::Revit::DB::Parameter^ p1,
                      Autodesk::Revit::DB::Parameter^ p2) {

    if (p1->Id->IntegerValue == p2->Id->IntegerValue)
      return (true);
    return (false);
  }

  virtual int GetHashCode(Autodesk::Revit::DB::Parameter^ p) {
    return (p->GetHashCode());
  }
};

[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)]
public ref class SelValuesCmd : Autodesk::Revit::UI::IExternalCommand {
public:
  virtual Autodesk::Revit::UI::Result Execute(
      Autodesk::Revit::UI::ExternalCommandData ^data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elements);
private:
  Autodesk::Revit::UI::Result CarryOutCmd(
      Autodesk::Revit::UI::ExternalCommandData ^data,
      System::String ^%msg, Autodesk::Revit::DB::ElementSet ^elements);
  void FillSetWithParameters(DB::Element^ elem,
                             Samolet::Helpers::SetOfValues^ set);
  void FillSetWithParametersInGroups(DB::Element^ elem,
                                     Samolet::Helpers::SetOfValues^ set);
  void ExtractParameters(Autodesk::Revit::UI::UIDocument ^uidoc,
                         array<Samolet::Helpers::SetOfValues^> ^sets);

  void	SetValues(Autodesk::Revit::DB::Document ^doc,
                  CL::Hashtable ^selected,
                  array<Samolet::Helpers::SetOfValues^> ^sets);

  void	SetEntities(
      CLG::IDictionary<System::String^,System::String^>^ name_id_dic,
      CLG::LinkedList<Autodesk::Revit::DB::Element^>^ rvt_elems);

  LinkedListElement^ DealWithMaterials(Autodesk::Revit::DB::Document^ doc);
};
}  // namespace Commands
}  // namespace Samolet

#endif  // SELECT_VALUES_CMD_H_
