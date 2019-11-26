// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_by_unique_id_form.h

#ifndef SELECT_BY_GUID_CMD_FORM_H_
# define SELECT_BY_GUID_CMD_FORM_H_

namespace Samolet {
namespace Gui {
  public ref class SelectByGuidForm : System::Windows::Forms::Form {
 public:
  SelectByGuidForm(void);
  
  array<System::String^>^ GetGuids(void);
 private:
  void InitializeComponent(void);
  void SetupLayout(void);
  void SetupButtons(void);
  void SetupLabel(void);
  void SetupTextBox(void);
  void OnShowClick(System::Object^ s, System::EventArgs^ e);
  void OnOkClick(System::Object^ s, System::EventArgs^ e);
  void OnCancelClick(System::Object^ s, System::EventArgs^ e);
  
  System::Windows::Forms::TextBox^		_id_txtbox;
  System::Windows::Forms::TableLayoutPanel^	_grid;
};
}  // namespace Gui
}  // namespace Samolet

#endif  // SELECT_BY_UNIQUE_ID_FORM_H_
