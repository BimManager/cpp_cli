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
            //InitializeListBox();
            InitializeButtons();
        }

        ViewsMgrForm::ViewsMgrForm(CL::IList ^names)
        {
            InitializeComponent();
            InitializeTableLayoutPanel();
            InitializeListBox(names);
            InitializeButtons();
        }

        void    ViewsMgrForm::InitializeComponent(void)
        {
            this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Text = L"Views Manager";
            this->AutoSize = true;
            this->Width = 300;
            this->Height = 300;
            //this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
            //this->AutoSizeMode = Forms::AutoSizeMode::;
        }

        void    ViewsMgrForm::InitializeTableLayoutPanel(void)
        {
            this->_layout = gcnew Forms::TableLayoutPanel();
            this->_layout->BackColor = DWG::SystemColors::Control;
            this->_layout->Dock = Forms::DockStyle::Fill;
            this->_layout->ColumnCount = 5;
            this->_layout->RowCount = 2;
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 20));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 20));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 20));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 20));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 20));                                                
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::Percent, 90));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::Percent, 10));
            this->Controls->Add(this->_layout);
        }

        void    ViewsMgrForm::InitializeListBox(CL::IList ^viewNames)
        {
            Forms::Label    ^lbl1;
            array<String ^> ^names;

            lbl1 = gcnew Forms::Label();
            lbl1->Text = "Foo";
            this->_lbviews = gcnew Forms::CheckedListBox();
            this->_viewNames = viewNames;
            this->FillListBox();
            this->_layout->Controls->Add(this->_lbviews, 0, 0);
            this->_lbviews->Sorted = true;
            this->_lbviews->Dock = Forms::DockStyle::Fill;
            this->_layout->SetColumnSpan(this->_lbviews, 5);
        }

        void    ViewsMgrForm::FillListBox(void)
        {
            CL::IEnumerator ^it;
            array<String ^> ^fields;

            it = this->_viewNames->GetEnumerator();
            while (it->MoveNext())
            {
                fields = ((String ^)it->Current)->Split(';');
                this->_lbviews->Items->Add(fields[0]);
            }
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
            this->InitializeButton(this->_btnSelectAll, 1, 1,
                    "Select All", gcnew EventHandler(this,
                    &ViewsMgrForm::OnSelectAllClicked));
            this->InitializeButton(this->_btnDiselectAll, 1, 2,
                    "Diselect All", gcnew EventHandler(this,
                    &ViewsMgrForm::OnDiselectAllClicked));
            this->InitializeButton(this->_btnNotOnSheets, 1, 2,
                    "Not On Sheets", gcnew EventHandler(this,
                    &ViewsMgrForm::OnNotOnSheetsClicked));                    
            this->InitializeButton(this->_btnCancel, 1, 4,
                    "Cancel", gcnew EventHandler(this,
                    &ViewsMgrForm::OnCancelClicked));                    
        }

        CL::IList   ^ViewsMgrForm::GetCheckedItems(void)
        {
            return (this->_checkedIndices);
        }

        void    ViewsMgrForm::OnDeleteClicked(System::Object ^s, System::EventArgs ^e)
        {
            CL::IEnumerator ^it;

            this->_checkedIndices = gcnew CL::ArrayList();
            it = this->_lbviews->CheckedIndices->GetEnumerator();
            while (it->MoveNext())
            {
                this->_checkedIndices->Add(it->ToString());
            }
            this->Close();
        }

        void    ViewsMgrForm::OnSelectAllClicked(System::Object ^s, System::EventArgs ^e)
        {
            size_t  i;

            i = this->_lbviews->Items->Count;
            while (i--)
                this->_lbviews->SetItemChecked(i, true);
        }

        void    ViewsMgrForm::OnDiselectAllClicked(System::Object ^s, System::EventArgs ^e)
        {
           size_t  i;

            i = this->_lbviews->Items->Count;
            while (i--)
                this->_lbviews->SetItemChecked(i, false);
        }

        void    ViewsMgrForm::OnNotOnSheetsClicked(System::Object ^s, System::EventArgs ^e)
        {
            size_t          i;
            array<String ^> ^fields;

            i = this->_lbviews->Items->Count;
            while (i--)
            {
                fields = ((String ^)this->_viewNames[i])->Split(';');
                if (fields[2] == "1")
                    this->_lbviews->SetItemChecked(i, false);
                else
                    this->_lbviews->SetItemChecked(i, true);
            }
        }

        void    ViewsMgrForm::OnCancelClicked(System::Object ^s, System::EventArgs ^e)
        {
            this->Close();
        }
    }
}