// Copyright 2019 Samolet LLC
// Author: kkozlov
// SetOfValues.h

#ifndef VALUES_SET_H
# define VALUES_SET_H

# include "selectable_value.h"
# include "utils.h"

namespace Samolet {
namespace Helpers {
ref class ParameterEqualityComparer : System::Collections::IEqualityComparer {
public:
  virtual bool Equals(System::Object^ obj1,
                      System::Object^ obj2) {
    Autodesk::Revit::DB::Parameter^ p1;
    Autodesk::Revit::DB::Parameter^ p2;

    if ((p1 = dynamic_cast<Autodesk::Revit::DB::Parameter^>(obj1)) != nullptr &&
        (p2 = dynamic_cast<Autodesk::Revit::DB::Parameter^>(obj2)) != nullptr) {
      if (p1->Id->IntegerValue == p2->Id->IntegerValue)
        return (true);
    }
    return (false);
  }

  virtual int GetHashCode(System::Object^ obj) {
    return (obj->GetHashCode());
  }
};
 
public ref class SetOfValues : IComparable {
 public:
  SetOfValues(String ^param_name);  
  SetOfValues(String ^param_name, array<String^> ^values);
  
  virtual int		CompareTo(Object ^obj);
  
  String^		GetParamName(void);
  SelectableValue^	GetValue(String ^valueName);
  array<String^>^	GetValuesAsString(void);
  String^		GetDefaultValue(void);
  LinkedListParameter^	GetRvtParams(void);
  bool			CanVaryByGroup(void);
  
  void 			AddRvtParam(DB::Parameter ^rvtParam);
  void			AddRvtElem(DB::Element^ elem);
  void			AddValue(SelectableValue ^val);
  static void		set_rvt_elems(CLG::LinkedList<DB::Element^>^ elems);
  static CLG::LinkedList<DB::Element^>^ rvt_elems(void);

  static void		SetSelectedElemsCount(int nElems);
  static int		GetSelectedElemsCount(void);
  
 private:
  void Initialize(System::String^ param_name);
  void FillCannotChangeByGroup(void);
  
  System::String	                ^_paramName;
  System::String			^_defaultValue;
  CLG::LinkedList<SelectableValue^>	^_possibleValues;
  CL::Hashtable^			_type_params;				
  LinkedListParameter^			_rvtParams;
  bool					_can_vary_by_group;
  static LinkedListElement^		_rvt_elems;
  static int				_selectedElemsCount;
  static IDictionaryStringString^	_cannot_vary_by_group;
};
}  // namespace HelperClasses
}  // namespace Samolet

#endif
