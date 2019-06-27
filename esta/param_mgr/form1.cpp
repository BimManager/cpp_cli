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
    this->_lblFilename = gcnew Forms::Label();
    this->InitializeLabel(this->_lblFilename);
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

void    FilePicker::InitializeLabel(Forms::Label ^lbl)
{
    lbl->Text = System::String::Empty;
    lbl->Location = DWG::Point(10, 10);
    lbl->Size = DWG::Size(150, 35);
    this->Controls->Add(lbl);
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
    if (saveDlg->ShowDialog() == Forms::DialogResult::OK)
    {
        this->_lblFilename->Text = 
            (gcnew System::IO::FileInfo(saveDlg->FileName))->FullName;
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
        this->_lblFilename->Text =
            (gcnew System::IO::FileInfo(openDlg->SafeFileName))->FullName;
    }
}

void FilePicker::OnImportClicked(System::Object ^s, System::EventArgs ^e)
{
    this->BringUpOpenDialog();
    if (this->_lblFilename->Text->Length)
    {
        this->Close();
        this->OnDialogDismissed(
            gcnew DismissedDialogEventArgs(
                this->_lblFilename->Text, ACTION_IMPORT));
    }
}

void FilePicker::OnExportClicked(System::Object ^s, System::EventArgs ^e)
{
    this->BringUpSaveDialog();
    if (this->_lblFilename->Text->Length)
    {
        this->Close();
        this->OnDialogDismissed(
            gcnew DismissedDialogEventArgs(
                this->_lblFilename->Text, ACTION_EXPORT));
    }
}

void FilePicker::OnCancelClicked(System::Object ^s, System::EventArgs ^e)
{
    this->Close();
}

