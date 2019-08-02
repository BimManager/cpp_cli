/*
 *  prmsmgr_form.cpp
 */

#include "prmsmgr_form.h"

using namespace Esta::Gui;
using namespace Esta::EventData;

DismissedDialogEventArgs::DismissedDialogEventArgs(System::String ^filepath, char action)
{
    this->_filepath = filepath;
    this->_action = action;
}

System::String  ^DismissedDialogEventArgs::GetFilepath(void)
{
    return (this->_filepath);
}

char  DismissedDialogEventArgs::GetAction(void)
{
    return (this->_action);
}

FilePicker::FilePicker()
{
    InitializeComponent();
}

void FilePicker::InitializeComponent()
{
    this->AutoSize = true;
    this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
    this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
    this->Text = L"Import/Export Shared Parameters";
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->MinimumSize = DWG::Size(400, 150);
    this->InitializeTableLayout(this->_tab);
    this->InitializeButtons();
    this->InitializeTextBox();
    this->_filepath = System::String::Empty;
}

void    FilePicker::InitializeTableLayout(Forms::TableLayoutPanel ^%tab)
{
    tab = gcnew Forms::TableLayoutPanel();
    tab->Dock = Forms::DockStyle::Fill;
    tab->Location = DWG::Point(0,0);
    tab->BackColor = DWG::SystemColors::Control;
    tab->ColumnCount = 2;
    tab->RowCount = 3;
    tab->ColumnStyles->Add(
        gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 100 / tab->ColumnCount));
    tab->ColumnStyles->Add(
        gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 100 / tab->ColumnCount));
    tab->RowStyles->Add(
        gcnew Forms::RowStyle(Forms::SizeType::Percent, 100 / tab->RowCount));
    tab->RowStyles->Add(
        gcnew Forms::RowStyle(Forms::SizeType::Percent, 100 / tab->RowCount));
    tab->RowStyles->Add(
        //gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
        gcnew Forms::RowStyle(Forms::SizeType::Percent, 100 / tab->RowCount));
    this->Controls->Add(this->_tab);
}

void    FilePicker::InitializeTextBox(void)
{
    Forms::TextBox  ^tb;

    tb = gcnew Forms::TextBox();
    tb->Dock = Forms::DockStyle::Fill;
    tb->Multiline = true;
    tb->TextAlign = Forms::HorizontalAlignment::Center;
    //tb->TextAlign = Forms::VerticalAlignment::Center;
    tb->ReadOnly = true;
    this->_tab->Controls->Add(tb, 0, 1);
    this->_txtBox = tb;
    this->_tab->SetColumnSpan(this->_txtBox, this->_tab->ColumnCount);
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
    InitializeButton(L"Import",
        0, 2, gcnew System::EventHandler(this, &FilePicker::OnImportClicked));
    InitializeButton(L"Export",
        1, 2, gcnew System::EventHandler(this, &FilePicker::OnExportClicked));
    InitializeButton(L"Cancel",
        1, 0, gcnew System::EventHandler(this, &FilePicker::OnCancelClicked));
}

void    FilePicker::OnDialogDismissed(DismissedDialogEventArgs ^e)
{
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
    {
        this->_filepath = saveDlg->FileName;
    }
}

void    FilePicker::BringUpOpenDialog(void)
{
    Forms::OpenFileDialog   ^openDlg;

    openDlg = gcnew Forms::OpenFileDialog();
    openDlg->CheckFileExists = true;
    openDlg->Filter = "txt files (*.txt)|*.txt";
    if (openDlg->ShowDialog() == Forms::DialogResult::OK)
    {
        this->_filepath = openDlg->FileName;
    }
}

void    FilePicker::OnSelectModelClicked(System::Object ^s, System::EventArgs ^e)
{
    FileOpenDialog  ^openDlg;
    ModelPath       ^path;

    openDlg = gcnew FileOpenDialog("rvt files (*.rvt)|*.rvt");
    if (openDlg->Show() == ItemSelectionDialogResult::Confirmed)
    {
        path = openDlg->GetSelectedModelPath();
        this->_txtBox->Text = 
            ModelPathUtils::ConvertModelPathToUserVisiblePath(path);
    }
}

void FilePicker::OnImportClicked(System::Object ^s, System::EventArgs ^e)
{
    this->BringUpOpenDialog();
    if (this->_filepath->Length)
    {
        this->Close();
        this->OnDialogDismissed(
            gcnew DismissedDialogEventArgs(
                this->_filepath, ACTION_IMPORT));
    }
}

void FilePicker::OnExportClicked(System::Object ^s, System::EventArgs ^e)
{
    this->BringUpSaveDialog();
    if (this->_filepath->Length)
    {
        this->Close();
        this->OnDialogDismissed(
            gcnew DismissedDialogEventArgs(
                this->_filepath, ACTION_EXPORT));
    }
}

void FilePicker::OnCancelClicked(System::Object ^s, System::EventArgs ^e)
{
    this->Close();
}

