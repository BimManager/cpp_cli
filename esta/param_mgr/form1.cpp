/*
 *  form1.cpp
 */

#include "form1.h"

using namespace Esta::Gui;

FilePicker::FilePicker()
{
    InitializeComponent();
}

void FilePicker::InitializeComponent()
{
    this->Size = DWR::Size(300, 300);
    this->Text = L"Select File";

    this->_btnSelFile = gcnew Forms::Button();
    this->_btnSelFile->Text = L"Pick file";
    this->_btnSelFile->Click += 
        gcnew System::EventHandler(this, &FilePicker::BtnOnClicked);

    this->Controls->Add(this->_btnSelFile);
}

void FilePicker::BtnOnClicked(System::Object ^s, System::EventArgs ^e)
{
    Forms::OpenFileDialog   ^openDlg;

    openDlg = gcnew Forms::OpenFileDialog();
    openDlg->CheckFileExists = true;
    if (openDlg->ShowDialog() == Forms::DialogResult::OK)
        this->Text = openDlg->SafeFileName();
}


