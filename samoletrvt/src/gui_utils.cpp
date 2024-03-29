// Copyright 2019 Samolet LLC
// Author: kkozlov
// GuiUtils.cpp

#include "gui_utils.h"

namespace Samolet {
namespace Gui {
namespace sys = System;
namespace ui = Autodesk::Revit::UI;
namespace db = Autodesk::Revit::DB;
namespace winforms = System::Windows::Forms;

sys::String^ GuiUtils::OpenFile(sys::String ^filter) {
  ui::FileOpenDialog^	open_dialog;;
  
  open_dialog = gcnew ui::FileOpenDialog(filter);
  if (open_dialog->Show() == ui::ItemSelectionDialogResult::Confirmed)
    return (db::ModelPathUtils::ConvertModelPathToUserVisiblePath(
        open_dialog->GetSelectedModelPath()));
  return (nullptr);
}

sys::String^ GuiUtils::SaveFile(sys::String ^filter,
                                sys::String ^init_filename) {
  ui::FileSaveDialog	^save_dialog;
  
  save_dialog = gcnew UI::FileSaveDialog(filter);
  save_dialog->InitialFileName = init_filename;
  if (save_dialog->Show() == UI::ItemSelectionDialogResult::Confirmed)
    return (DB::ModelPathUtils::ConvertModelPathToUserVisiblePath(
        save_dialog->GetSelectedModelPath()));
  return (nullptr);
}

void GuiUtils::SetupForm(Forms::Form ^form, String ^text) {
  form->AutoSizeMode =
      Forms::AutoSizeMode::GrowAndShrink;
  form->Text = text;
  form->AutoSize = true;
  form->FormBorderStyle =
      Forms::FormBorderStyle::FixedSingle;
   form->AutoSizeMode =
       Forms::AutoSizeMode::GrowOnly;
}

Forms::Button^ GuiUtils::CreateButton(
    String ^name, String ^text, EventHandler ^onClick) {
  Forms::Button	^btn;

  btn = gcnew Forms::Button();
  btn->Name = name;
  btn->Text = text;
  btn->Margin = Forms::Padding(5);
  btn->Click += onClick;
  btn->Dock = Forms::DockStyle::Fill;
  return (btn);
}

Forms::TableLayoutPanel^ GuiUtils::CreateLayout(
     String ^name, int nCols, int nRows) {
  Forms::TableLayoutPanel  ^grid;

  grid = gcnew Forms::TableLayoutPanel();
  grid->Name = name;
  grid->AutoSize = true;
  grid->AutoSizeMode = Forms::AutoSizeMode::GrowOnly;
  grid->Dock = Forms::DockStyle::Fill;
  grid->ColumnCount = nCols;
  grid->RowCount = nRows;
  grid->Margin = Forms::Padding(5);
  grid->ColumnStyles->Add(
      gcnew Forms::ColumnStyle(Forms::SizeType::AutoSize));
  grid->RowStyles->Add(
      gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
  return (grid);
}

Forms::Label^ GuiUtils::CreateLabel(
    String^ name, String^ text) {
  Forms::Label      ^lb;

  lb = gcnew Forms::Label();
  lb->AutoSize = true;
  lb->Name = name;
  lb->Text = text;
  lb->Dock = Forms::DockStyle::Fill;
  lb->TextAlign = Dwg::ContentAlignment::MiddleLeft;
  lb->Margin = Forms::Padding(5);
  return (lb);
}

Forms::ComboBox^ GuiUtils::CreateComboBox(
      String^ name, array<String^>^ values) {
  Forms::ComboBox^			cbx;
  Forms::AutoCompleteStringCollection^	strCol;

  cbx = gcnew Forms::ComboBox();
  cbx->AutoSize = true;
  cbx->Name = name;
  cbx->Items->AddRange(values);
  if (values->Length)
    cbx->SelectedIndex = 0;
  cbx->AutoCompleteMode =
      Forms::AutoCompleteMode::SuggestAppend;
  cbx->DropDownStyle =
      Forms::ComboBoxStyle::DropDown;
  cbx->AutoCompleteSource =
      Forms::AutoCompleteSource::CustomSource;
  strCol = gcnew Forms::AutoCompleteStringCollection();
  strCol->AddRange(values);
  cbx->AutoCompleteCustomSource = strCol;
  cbx->Dock = Forms::DockStyle::Fill;
  cbx->Margin = Forms::Padding(5);
  return (cbx);
}

Forms::TextBox^ GuiUtils::CreateTextBox(String^ name) {
  Forms::TextBox^	tb;

  tb = gcnew Forms::TextBox();
  tb->AutoSize = true;
  tb->Name = name;
  tb->Text = String::Empty;
  tb->Dock = Forms::DockStyle::Fill;
  tb->Margin = Forms::Padding(5);
  return (tb);
}

Forms::TextBox^ GuiUtils::CreatePasswordTextBox(String^ name) {
  Forms::TextBox^	tb;

  tb = CreateTextBox(name);
  tb->PasswordChar = '*';
  tb->MaxLength = 8;
  return (tb);
}

PasswordForm::PasswordForm(System::String^ passwd) {
  passwd_ = Utils::HandyUtils::StringToSecureString(passwd);
  IsCorrectPassword = false;
  SetupForm();
  SetupLayout();
  SetupTextBoxButtons();
}

void	PasswordForm::SetupForm(void) {
  GuiUtils::SetupForm(this, "Пароль");
  this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
  this->MaximizeBox = false;
  this->MinimizeBox = false;
  this->StartPosition = Forms::FormStartPosition::CenterScreen;
}
void	PasswordForm::SetupLayout(void) {
  System::Windows::Forms::TableLayoutPanel^	layout;
  
  layout = GuiUtils::CreateLayout("grid", 2, 3);
  this->Controls->Add(layout);
  layout_ = layout;
}
void PasswordForm::SetupTextBoxButtons(void) {
  System::Windows::Forms::TextBox^	tb;
  System::Windows::Forms::Button^	btn;
  System::Windows::Forms::Label^	lb;

  lb = GuiUtils::CreateLabel("passwd", "Пароль:");
  layout_->Controls->Add(lb, 0, 0);
  tb = GuiUtils::CreatePasswordTextBox("passwd_txtbx");
  layout_->Controls->Add(tb, 0, 1);
  layout_->SetColumnSpan(tb, 2);
  passwd_box_ = tb;
  btn = GuiUtils::CreateButton(
      "ok_btn", "OK",
      gcnew System::EventHandler(this, &PasswordForm::OnOkClicked));
  layout_->Controls->Add(btn, 0, 2);
  btn->Height = 15;
  btn = GuiUtils::CreateButton(
      "cancel_btn", "Отмена",
      gcnew System::EventHandler(this, &PasswordForm::OnCancelClicked));
  layout_->Controls->Add(btn, 1, 2);
}
void PasswordForm::OnOkClicked(System::Object^ s, System::EventArgs^ e) {
  String^	passwd;
  
  passwd = Utils::HandyUtils::SecureStringToString(passwd_);
#ifdef DEBUG
  Debug::WriteLine(String::Format(
      "[RVT {0}] {1} == {2}",
      __FUNCTION__, passwd_box_->Text, passwd));
#endif
  if (!System::String::Compare(passwd, passwd_box_->Text)) {
    IsCorrectPassword = true;
    this->Close();
  } else {
    System::Windows::Forms::MessageBox::Show(
        "Неверный пароль", "Ошибка",
        Forms::MessageBoxButtons::OK, Forms::MessageBoxIcon::Error);
  }
}
void PasswordForm::OnCancelClicked(System::Object^ s, System::EventArgs^ e) {
  this->Close();
}
} // namespace Gui
} // namespace Samolet
