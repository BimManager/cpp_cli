/*
 *  form1.cpp
 */

#include "form1.h"

Forms::Form1::Form1()
{
    InitializeComponent();
}

Forms::Form1::~Form1()
{
    if (this->_components)
        delete this->_components;
}

void    Forms::Form1::InitializeComponent()
{
    this->_label = gcnew System::Windows::Forms::Label();
    this->_label->Text = L"Click me!";
    this->_label->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
    this->_label->Location = System::Drawing::Point(90, 90);
    this->_label->Click += gcnew System::EventHandler(this, &Forms::Form1::LabelClick);
    this->SuspendLayout();

    this->_components = gcnew System::ComponentModel::Container();
    this->Size = System::Drawing::Size(300, 300);
    this->Text = L"Form1";
    this->Padding = System::Windows::Forms::Padding(0);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->Controls->Add(this->_label);
}

void    Forms::Form1::LabelClick(System::Object ^s, System::EventArgs ^e)
{
    System::Windows::Forms::OpenFileDialog  ^openDlg;

    this->_label->Text = L"I have just been clicked!";
    openDlg = gcnew System::Windows::Forms::OpenFileDialog();
    openDlg->ShowDialog();
}