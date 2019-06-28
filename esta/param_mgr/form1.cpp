/*
 *  form1.cpp
 */

#include "form1.h"

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
    Forms::Button   ^btn;

    this->Size = DWG::Size(300, 300);
    this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
    this->Text = L"Import/Export Shared Parameters";
    this->HelpButton = true;
    this->MaximizeBox = false;
    this->InitializeButtons();
    this->_filepath = System::String::Empty;
}

void    FilePicker::InitializeButton(Forms::Button ^btn, System::String ^title,
            DWG::Point pos, System::EventHandler ^handler)
        {
            btn->Text = title;
            btn->Location = pos;
            btn->Click += handler;
            this->Controls->Add(btn);
        }

void    FilePicker::InitializeButtons(void)
{
    this->_btnImport = gcnew Forms::Button();
    this->_btnExport = gcnew Forms::Button();
    this->_btnCancel = gcnew Forms::Button();
    InitializeButton(this->_btnImport, L"Import",
        DWG::Point(10, 50), gcnew System::EventHandler(this, &FilePicker::OnImportClicked));
    InitializeButton(this->_btnExport, L"Export",
        DWG::Point(10, 100), gcnew System::EventHandler(this, &FilePicker::OnExportClicked));
    InitializeButton(this->_btnCancel, L"Cancel",
        DWG::Point(10, 150), gcnew System::EventHandler(this, &FilePicker::OnCancelClicked));
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

