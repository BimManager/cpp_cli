/*
 *  viewsmgr_form.cpp
 */

#include "viewsmgr_form.h"

namespace Esta
{
    namespace Gui
    {
        ViewsMgrForm::ViewsMgrForm(void)
        {
            InitializeComponent();
            InitializeTableLayoutPanel();
            InitializeListBox();
            InitializeButtons();
        }

        void    ViewsMgrForm::InitializeComponent(void)
        {
            this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Text = L"Views Manager";
            this->AutoSize = true;
            this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
        }

        void    ViewsMgrForm::InitializeTableLayoutPanel(void)
        {
            this->_layout = gcnew Forms::TableLayoutPanel();
            this->_layout->BackColor = DWG::SystemColors::Control;
            this->_layout->ColumnCount = 2;
            this->_layout->RowCount = 2;
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::Percent, 70));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::Percent, 30));
            this->Controls->Add(this->_layout);
        }

        void    ViewsMgrForm::InitializeListBox(void)
        {
            Forms::Label    ^lbl1;

            lbl1 = gcnew Forms::Label();
            lbl1->Text = "Foo";
            this->_lbviews = gcnew Forms::CheckedListBox();
            this->_lbviews->Items->Add(lbl1->Text);
            this->_layout->Controls->Add(this->_lbviews, 0, 0);
            this->_lbviews->Sorted = true;
            this->_lbviews->Dock = Forms::DockStyle::Fill;
            this->_layout->SetColumnSpan(this->_lbviews, 2);
        }

        void    ViewsMgrForm::InitializeButton(
                    Forms::Button ^%btn, 
                    int row, int col,
                    String ^text, EventHandler ^handler)
        {
            btn = gcnew Forms::Button();
            btn->Text = text;
            btn->Click += handler;
            btn->Dock = Forms::DockStyle::Fill;
            this->_layout->Controls->Add(btn, col, row);
        }

        void    ViewsMgrForm::InitializeButtons(void)
        {
            this->InitializeButton(this->_btnDelete, 1, 0,
                    "Delete", gcnew EventHandler(this,
                    &ViewsMgrForm::OnDeleteClicked));
            this->InitializeButton(this->_btnCancel, 1, 1,
                    "Cancel", gcnew EventHandler(this,
                    &ViewsMgrForm::OnCancelClicked));
        }

        void    ViewsMgrForm::OnDeleteClicked(System::Object ^s, System::EventArgs ^e)
        {
            Forms::MessageBox::Show("Under development", "Gen");
        }

        void    ViewsMgrForm::OnCancelClicked(System::Object ^s, System::EventArgs ^e)
        {
            this->Close();
        }
    }
}