// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_by_unique_id_form.cpp

#include "select_by_unique_id_form.h"

#include "gui_utils.h"

namespace Samolet {
namespace Gui {
SelectByGuidForm::SelectByGuidForm(void) {
  InitializeComponent();
  SetupLayout();
  SetupButtons();
  SetupLabel();
  SetupTextBox();
}

void SelectByGuidForm::InitializeComponent(void) {
  GuiUtils::SetupForm(this, "Поиск по unique id");
  this->MinimumSize = DWG::Size(310, 0);
}

void SelectByGuidForm::SetupLayout(void) {
  _grid = GuiUtils::CreateLayout("grid", 3, 3);
  this->Controls->Add(_grid);
}

void SelectByGuidForm::SetupButtons(void) {
  WINFORMS::Button	^btn;

  btn = GuiUtils::CreateButton(
      "show_btn", "Показать", gcnew EventHandler(this,
          &SelectByGuidForm::OnShowClick));
  //  btn->Margin = WINFORMS::Padding(5);
  _grid->Controls->Add(btn, 0, 2);
  btn = GuiUtils::CreateButton(
      "ok_btn", "OK", gcnew
      EventHandler(this,
          &SelectByGuidForm::OnOkClick));
  //  btn->Margin = WINFORMS::Padding(5);
  _grid->Controls->Add(btn, 1, 2);
  btn = GuiUtils::CreateButton(
      "cancel_btn", "Отмена", gcnew
      EventHandler(this,
          &SelectByGuidForm::OnCancelClick));
  //  btn->Margin = WINFORMS::Padding(5);
  _grid->Controls->Add(btn, 2, 2);
}

void SelectByGuidForm::SetupLabel(void) {
  WINFORMS::Label	^lb;

  lb = gcnew WINFORMS::Label();
  lb->Text = "ID-(используйте точку с запятой " +
      "для нескольких ID):";
  lb->Dock = WINFORMS::DockStyle::Fill;
  lb->TextAlign = DWG::ContentAlignment::MiddleLeft;
  _grid->Controls->Add(lb, 0, 0);
  _grid->SetColumnSpan(lb, 3);
}

void SelectByGuidForm::SetupTextBox(void) {
  WINFORMS::TextBox	^txtBox;

  txtBox = gcnew WINFORMS::TextBox();
  txtBox->Name = "guids";
  txtBox->Dock = WINFORMS::DockStyle::Fill;
  _grid->Controls->Add(txtBox, 0, 1);
  _grid->SetColumnSpan(txtBox, 3);
  _id_txtbox = txtBox;
}

void SelectByGuidForm::OnShowClick(Object ^s,
                                   EventArgs ^e) {
  if (_id_txtbox->Text->Length)
    this->Close();
}

void SelectByGuidForm::OnOkClick(Object ^s,
                                 EventArgs ^e) {
  if (_id_txtbox->Text->Length)
    this->Close();
}
      
void SelectByGuidForm::OnCancelClick(Object ^s,
                                     EventArgs ^e) {
  this->Close();
}

array<String^>^ SelectByGuidForm::GetGuids(void) {
  if (_id_txtbox->Text->Length)
    return (_id_txtbox->Text->Split(';'));
  return (nullptr);
}

} // namespace Gui
} // namespace Samolet

