// Copyright 2019 Samolet LLC
// Author: kkozlov
// shared_params_manager_form.cpp

#include "shared_params_manager_form.h"

#include "gui_utils.h"

namespace Samolet {
namespace  eventdata {
namespace sys = System;
namespace db = Autodesk::Revit::DB;

DismissedDialogEventArgs::DismissedDialogEventArgs(
    sys::String^ filepath, db::ModelPath^ modelpath, char action)
    : filepath_{filepath}, modelpath_{modelpath}, action_{action} {}

sys::String^	DismissedDialogEventArgs::Filepath(void) {
    return (filepath_);
}

db::ModelPath^	DismissedDialogEventArgs::Modelpath(void) {
    return (modelpath_);
}

char  DismissedDialogEventArgs::Action(void) {
    return (action_);
}
}  // namespace eventdata

namespace gui {
namespace sys = System;
namespace db = Autodesk::Revit::DB;
namespace dwg = System::Drawing;
namespace winforms = System::Windows::Forms;

FilePicker::FilePicker() {
    InitializeComponent();
}

void FilePicker::InitializeComponent() {
    this->AutoSize = true;
    this->AutoSizeMode = winforms::AutoSizeMode::GrowAndShrink;
    this->FormBorderStyle = winforms::FormBorderStyle::FixedDialog;
    this->Text = L"Import/Export Shared Parameters";
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->MinimumSize = dwg::Size(400, 150);
    this->InitializeTableLayout(_tab);
    this->InitializeButtons();
    this->InitializeTextBox();
    this->filepath_ = sys::String::Empty;
}

void    FilePicker::InitializeTableLayout(Forms::TableLayoutPanel ^%tab)
{
    tab = gcnew winforms::TableLayoutPanel();
    tab->Dock = winforms::DockStyle::Fill;
    tab->Location = dwg::Point(0,0);
    tab->BackColor = dwg::SystemColors::Control;
    tab->ColumnCount = 2;
    tab->RowCount = 3;
    tab->ColumnStyles->Add(
        gcnew Forms::ColumnStyle(winforms::SizeType::Percent, 100 / tab->ColumnCount));
    tab->ColumnStyles->Add(
        gcnew Forms::ColumnStyle(winforms::SizeType::Percent, 100 / tab->ColumnCount));
    tab->RowStyles->Add(
        gcnew Forms::RowStyle(winforms::SizeType::Percent, 100 / tab->RowCount));
    tab->RowStyles->Add(
        gcnew Forms::RowStyle(winforms::SizeType::Percent, 100 / tab->RowCount));
    tab->RowStyles->Add(
        //gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
        gcnew Forms::RowStyle(Forms::SizeType::Percent, 100 / tab->RowCount));
    this->Controls->Add(_tab);
}

void    FilePicker::InitializeTextBox(void)
{
  winforms::TextBox  ^tb;

    tb = gcnew Forms::TextBox();
    tb->Dock = Forms::DockStyle::Fill;
    tb->Multiline = true;
    tb->TextAlign = Forms::HorizontalAlignment::Center;
    //tb->TextAlign = Forms::VerticalAlignment::Center;
    tb->ReadOnly = true;
    this->_tab->Controls->Add(tb, 0, 1);
    this->_txtbox = tb;
    this->_tab->SetColumnSpan(_txtbox, _tab->ColumnCount);
}

void    FilePicker::InitializeButton(System::String ^title,
            int col, int row, System::EventHandler ^handler)
{
    Forms::Button   ^btn;

    btn = gcnew Forms::Button();
    btn->Text = title;
    btn->Dock = Forms::DockStyle::Fill;
    btn->Click += handler;
    this->_tab->Controls->Add(btn, col, row);
}

void    FilePicker::InitializeButtons(void)
{
    InitializeButton(L"Select Model",
        0, 0, gcnew System::EventHandler(this, &FilePicker::OnSelectModelClicked));
    InitializeButton(L"Cancel",
        1, 0, gcnew System::EventHandler(this, &FilePicker::OnCancelClicked));
    InitializeButton(L"Import",
        0, 2, gcnew System::EventHandler(this, &FilePicker::OnImportClicked));
    InitializeButton(L"Export",
        1, 2, gcnew System::EventHandler(this, &FilePicker::OnExportClicked));
}

void    FilePicker::OnDialogDismissed(eventdata::DismissedDialogEventArgs ^e){
    this->DialogDismissed(this, e);
}

void    FilePicker::BringUpSaveDialog(void)
{
    Forms::SaveFileDialog   ^saveDlg;

    saveDlg = gcnew Forms::SaveFileDialog();
    saveDlg->Filter = "txt files (*.txt)|*txt";
    saveDlg->DefaultExt = "txt";
    saveDlg->RestoreDirectory = 0;
    if (saveDlg->ShowDialog() == Forms::DialogResult::OK)
        this->filepath_ = saveDlg->FileName;
}

void    FilePicker::BringUpOpenDialog(void)
{
    Forms::OpenFileDialog   ^openDlg;

    openDlg = gcnew Forms::OpenFileDialog();
    openDlg->CheckFileExists = true;
    openDlg->Filter = "txt files (*.txt)|*.txt";
    if (openDlg->ShowDialog() == Forms::DialogResult::OK)
        this->filepath_ = openDlg->FileName;
}

void    FilePicker::OnSelectModelClicked(
            System::Object ^s, System::EventArgs ^e) {
  namespace gui_utils = Samolet::Gui;
  String^	filepath;

  if ((filepath = gui_utils::GuiUtils::OpenFile("rvt files (*.rvt)|*.rvt"))) {
    _txtbox->Text = filepath;
    modelpath_ = db::ModelPathUtils::
        ConvertUserVisiblePathToModelPath(filepath);
  }

  /*  FileOpenDialog  ^openDlg;
  db::ModelPath^	path;
    
    openDlg = gcnew FileOpenDialog("rvt files (*.rvt)|*.rvt");
    if (openDlg->Show() == ItemSelectionDialogResult::Confirmed)
    {
        path = openDlg->GetSelectedModelPath();
        this->_txtBox->Text = 
            ModelPathUtils::ConvertModelPathToUserVisiblePath(path);
        this->_modelPath = path;
        }*/
}

void    FilePicker::ExportImport(char action1, char action2, VoidDelegateVoid ^pfn) {
    if (modelpath_ == nullptr) {
        pfn();
        if (sys::String::Compare(filepath_, sys::String::Empty))
          HandleExportImport(action1);
    } 
    else 
        HandleExportImport(action2);
    /*    if (modelpath_ != nullptr || 
        sys::String::Compare(filepath_, sys::String::Empty))
        Close();*/ 
}

void    FilePicker::HandleExportImport(char action) {
  eventdata::DismissedDialogEventArgs^	args;

  args = gcnew eventdata::DismissedDialogEventArgs(
      filepath_, modelpath_, action);
  OnDialogDismissed(args);            
}

void FilePicker::OnImportClicked(sys::Object ^s, sys::EventArgs ^e) {
    ExportImport(ACTION_IMPORT_FROM_FILE,
        ACTION_IMPORT_FROM_MODEL, 
        gcnew VoidDelegateVoid(this, &FilePicker::BringUpOpenDialog));
}

void FilePicker::OnExportClicked(sys::Object ^s, sys::EventArgs ^e) {
    ExportImport(ACTION_EXPORT_TO_FILE,
        ACTION_EXPORT_TO_MODEL,
        gcnew VoidDelegateVoid(this, &FilePicker::BringUpSaveDialog));
}

void FilePicker::OnCancelClicked(System::Object ^s, System::EventArgs ^e) {
    Close();
}
}  // namespace gui
}  // namespace Samolet
