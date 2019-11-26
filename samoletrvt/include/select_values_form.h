// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_values_form.h

#ifndef SELECT_VALUES_FORM_H_
# define SELECT_VALUES_CMD_FORM_H_

# include "set_of_values.h"

namespace Samolet {
namespace Gui {
using Samolet::Helpers::SetOfValues;

[Serializable]
public ref class SelectionForm : Forms::Form {
public:
  SelectionForm(array<SetOfValues^> ^sets);
  SelectionForm(array<SetOfValues^> ^sets, bool check);

  CL::Hashtable   ^GetSelected(void);
private:
  CL::Hashtable^		_selected;
  Forms::TableLayoutPanel^	_outPanel;
  Forms::TableLayoutPanel^	_panel;
  int				_cFields;
  bool				_suppress_check = false;

  void Initialize(array<SetOfValues^>^ sets) {
    _selected = gcnew CL::Hashtable();
    InitializeComponent();
    SetupOuterPanel();
    SetupPanel();
    PopulateView(sets);
    SetupButtons();
  }
  void    InitializeComponent(void);
  void    SetupOuterPanel(void);
  void    SetupPanel(void);
  void    SetupButtons(void);
  
  void    PopulateView(array<SetOfValues^> ^sets);

  void    AddRow(String ^pn, array<String^> ^vals, int row, int defIndex);
  void    FillSelected(void);

  void
      OnLoadForm(Object ^s, EventArgs ^e);
  void    OnOkClicked(Object ^s, EventArgs ^e);
  void    OnCancelClicked(Object ^s, EventArgs ^e);
};
}  // namespace Gui
}  // namespace Samolet

#endif
