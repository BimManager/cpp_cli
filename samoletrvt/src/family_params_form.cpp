// Copyright 2019 Samolet LLC
// Author: kkozlov
// family_params_form.cpp

#include "family_params_form.h"

#include "gui_utils.h"
#include "utils.h"

namespace Samolet {
namespace Gui {
namespace sys = System;
namespace winforms = System::Windows::Forms;
namespace io = System::IO;
namespace txt = System::Text;
namespace utils = Samolet::Utils;

ParamsControllerForm::ParamsControllerForm(void) {
  InitializeComponent();
  SetupLayout();
  SetupLabels();
  SetupComboBoxes();
  SetupButtons();
}

Samolet::Commands::FamilyParamDefinition^
ParamsControllerForm::GetParamDefinition(void) {
  return (_paramDef);
}

void ParamsControllerForm::InitializeComponent(void) {
  GuiUtils::SetupForm(this, "Удаление параметров");
  this->Width = 350;
}

void	ParamsControllerForm::SetupLayout(void) {
  Forms::TableLayoutPanel^	layout;

  layout = GuiUtils::CreateLayout(
      "layout", 2, 5);
  this->Controls->Add(layout);
}

void	ParamsControllerForm::SetupLabels(void) {
  winforms::Label		^lb;
  winforms::TableLayoutPanel	^grid;

  grid = static_cast<Forms::TableLayoutPanel^>(
      this->Controls->Find("layout", true)[0]);
  lb = GuiUtils::CreateLabel("pn", "Имя параметра:");
  grid->Controls->Add(lb, 0, 0);
  lb = GuiUtils::CreateLabel("pt", "Тип параметра:");
  grid->Controls->Add(lb, 0, 1);
  lb = GuiUtils::CreateLabel("pg", "Группа параметра:");
  grid->Controls->Add(lb, 0, 2);
  lb = GuiUtils::CreateLabel("inst", "Экземпляр:");
  grid->Controls->Add(lb, 0, 3);
}

void	ParamsControllerForm::SetupComboBoxes(void) {
  winforms::ComboBox		^cbx;
  winforms::TextBox		^txtbox;
  winforms::TableLayoutPanel	^grid;
  array<String^>		^tmp;
  array<String^>		^components;
  
  if ((tmp =  utils::FileUtils::ReadNLines(FILE_PATH, 1)))
    components = tmp[0]->Split(';');
  grid = static_cast<Forms::TableLayoutPanel^>(
      this->Controls->Find("layout", true)[0]);
  cbx = GuiUtils::CreateComboBox(
      "paramType", utils::HandyUtils::StringifyEnumValues(
          DB::ParameterType::typeid));
  cbx->DropDownStyle = Forms::ComboBoxStyle::DropDownList;
  if (components)
    cbx->SelectedItem = components[1];
  grid->Controls->Add(cbx, 1, 1);
  cbx = GuiUtils::CreateComboBox(
      "paramGroup", utils::HandyUtils::StringifyEnumValues(
          DB::BuiltInParameterGroup::typeid));
  cbx->DropDownStyle = Forms::ComboBoxStyle::DropDownList;
  grid->Controls->Add(cbx, 1, 2);
  if (components)
    cbx->SelectedItem = components[2];
  cbx = GuiUtils::CreateComboBox(
      "isInstance", gcnew array<String^> { "Yes", "No" });
  cbx->DropDownStyle = Forms::ComboBoxStyle::DropDownList;
  if (components)
    cbx->SelectedItem = components[3];
  grid->Controls->Add(cbx, 1, 3);
  txtbox = GuiUtils::CreateTextBox("paramName");
  if (components)
    txtbox->Text = components[0];
  grid->Controls->Add(txtbox);
}

void	ParamsControllerForm::SetupButtons(void) {
  winforms::TableLayoutPanel	^main_grid;
  winforms::TableLayoutPanel	^grid;
  Forms::Button^	btn;

  grid = GuiUtils::CreateLayout("button_layout", 2, 1);
  btn = GuiUtils::CreateButton(
      "delete", "Delete",
      gcnew EventHandler(this,
         &ParamsControllerForm::OnDeleteClicked));
  grid->Controls->Add(btn, 0, 0);
  btn = GuiUtils::CreateButton(
      "cancel", "Cancel",
      gcnew EventHandler(this,
         &ParamsControllerForm::OnCancelClicked));
  grid->Controls->Add(btn, 1, 0);
  grid->ColumnStyles[0] = gcnew winforms::ColumnStyle(
      winforms::SizeType::Percent, 50);
  grid->ColumnStyles->Add(gcnew winforms::ColumnStyle(
      winforms::SizeType::Percent, 50));
  grid->Margin = winforms::Padding(0);
  main_grid = static_cast<Forms::TableLayoutPanel^>(
      this->Controls->Find("layout", true)[0]);
  main_grid->Controls->Add(grid, 0, 4);
  main_grid->SetColumnSpan(grid, 2);
}

void	ParamsControllerForm::OnDeleteClicked(Object^ s, EventArgs^ e) {
  using Samolet::Commands::FamilyParamDefinition;
  DB::ParameterType		pt;
  DB::BuiltInParameterGroup	gt;
  sys::String^			line;

  pt = static_cast<DB::ParameterType>(
      Enum::Parse(DB::ParameterType::typeid,
                  static_cast<Forms::ComboBox^>(
                      this->Controls->Find("paramType", true)[0])
                  ->SelectedItem->ToString()));
  gt = static_cast<DB::BuiltInParameterGroup>(
      Enum::Parse(DB::BuiltInParameterGroup::typeid,
                  static_cast<Forms::ComboBox^>(
                      this->Controls->Find("paramGroup", true)[0])
                  ->SelectedItem->ToString()));
  _paramDef = gcnew FamilyParamDefinition (
      static_cast<Forms::TextBox^>(this->Controls->Find(
          "paramName", true)[0])->Text, pt, gt,
      String::Compare(static_cast<Forms::ComboBox^>(
          this->Controls->Find("isInstance", true)[0])
              ->SelectedItem->ToString(), "Yes") == 0 ? true : false);
  line = utils::FileUtils::ReadNLines(FILE_PATH, 1)[0];
  if (line == nullptr ||(line != nullptr &&
       String::Compare(line, _paramDef->ToString()) != 0))
    utils::FileUtils::SaveToFile(FILE_PATH, _paramDef->ToString());
#ifdef DEBUG
  Debug::WriteLine(FILE_PATH);
  Debug::WriteLine(_paramDef->ToString());
#endif
  this->Close();
}

void	ParamsControllerForm::OnCancelClicked(Object^ s, EventArgs^ e) {
  _paramDef = nullptr;
  this->Close();
}
} // namespace Gui
} // namespace Samolet
