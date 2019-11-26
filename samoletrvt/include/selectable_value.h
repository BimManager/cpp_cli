// Copyright 2019 Samolet LLC
// Author: kkozlov
// SelectableValue.h

#ifndef SELECTABLE_VALUE_H
# define SELECTABLE_VALUE_H

# define INVALID_ID	-1

namespace Samolet {
namespace Helpers {
public ref class SelectableValue : IComparable {
public:
  SelectableValue(String ^value);
  SelectableValue(String ^value, int id);
  SelectableValue(String ^value, int id,
                  String ^guid);
  
  virtual String^ ToString(void) override;
  virtual int CompareTo(Object ^other);
  String^ GetGuid(void);
  int GetId(void);
private:
  String^	_value;
  String^	_guid;
  int		_id;
};    
} // namespace HelperClasses
} // namespace Samolet
 
#endif
