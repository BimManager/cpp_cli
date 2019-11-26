// Copyright 2019 Samolet LLC
// Author: kkozlov
// GuiUtils.h

#ifndef GUI_UTILS_H_
# define GUI_UTILS_H_

#include "utils.h"

namespace Samolet {
namespace Gui {
public ref class GuiUtils {
 public:
  static System::String^ SaveFile(System::String^ filter,
                                  System::String^ init_filename);
  static System::String^ OpenFile(System::String ^filter);
  static void SetupForm(System::Windows::Forms::Form ^form,
                        System::String ^text);
  static System::Windows::Forms::Button^ CreateButton(
      System::String^ name, System::String^ text,
      System::EventHandler^ onClick);
  static System::Windows::Forms::TableLayoutPanel^ CreateLayout(
      System::String^ name, int n_cols, int n_rows);
  static System::Windows::Forms::Label^ CreateLabel(
      System::String^ name, System::String^ text);
  static System::Windows::Forms::TextBox^ CreateTextBox(
      System::String^ name);
  static System::Windows::Forms::ComboBox^ CreateComboBox(
      System::String^ name, array<System::String^>^ values);
  static System::Windows::Forms::TextBox^ CreatePasswordTextBox(
      System::String^ name);
};

public ref class PasswordForm : System::Windows::Forms::Form {
 public:
  PasswordForm(System::String^ passwd);
  property bool IsCorrectPassword;
 private:
  void	SetupForm(void);
  void	SetupLayout(void);
  void	SetupTextBoxButtons(void);
  
  void	OnOkClicked(System::Object^ s, System::EventArgs^ e);
  void	OnCancelClicked(System::Object^ s, System::EventArgs^ e);
  
  System::Security::SecureString^		passwd_;
  System::Windows::Forms::TableLayoutPanel^	layout_;
  System::Windows::Forms::TextBox^		passwd_box_;
};
}  // namespace Gui
}  // namespace Samolet
#endif  // GUI_UTILS_H_
