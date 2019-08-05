/*
 *  viewsmgr_form.cpp
 */

#include "viewsmgr_form.h"

namespace Esta
{
    namespace Gui
    {
        ViewsMgrForm::ViewsMgrForm(CL::IList ^names, CL::IList ^viewData)
        {
            //InitializeComponent();
            InitializeComponent(viewData);
            InitializeTableLayoutPanel();
            InitializeListBox(names);
            InitializeButtons();
        }

        void    ViewsMgrForm::InitializeComponent(void)
        {
            this->FormBorderStyle = Forms::FormBorderStyle::Sizable;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Text = L"Views Manager";
            this->MinimumSize = DWG::Size(300,300);
        }

        void    ViewsMgrForm::InitializeComponent(CL::IList ^viewData)
        {
            InitializeComponent();
            this->_viewData = viewData;
        }


        void    ViewsMgrForm::InitializeTableLayoutPanel(void)
        {
            this->_layout = gcnew Forms::TableLayoutPanel();
            this->_layout->Dock = Forms::DockStyle::Fill;
            this->_layout->ColumnCount = 2;
            this->_layout->RowCount = 7;
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
            this->_layout->RowStyles->Add(
                gcnew Forms::RowStyle(Forms::SizeType::AutoSize));                
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 100));
            this->_layout->ColumnStyles->Add(
                gcnew Forms::ColumnStyle(Forms::SizeType::Absolute, 100));
            this->Controls->Add(this->_layout);
        }

        void    ViewsMgrForm::InitializeListBox(CL::IList ^viewNames)
        {
            this->_lbviews = gcnew Forms::CheckedListBox();
            this->_viewNames = viewNames;
            this->_lbviews->Dock = Forms::DockStyle::Fill;
            /* this->_lbviews->SelectionMode = Forms::SelectionMode::MultiExtended; */
            this->_layout->Controls->Add(this->_lbviews, 0, 0);
            this->_layout->SetRowSpan(this->_lbviews, this->_layout->RowCount);
            //this->FillListBox();
            this->FillListBoxVD();
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

        void    ViewsMgrForm::FillListBoxVD(void)
        {
            CL::IEnumerator ^it;
            ViewData        ^vd;

            it = this->_viewData->GetEnumerator();
            while (it->MoveNext())
            {
                vd = (ViewData ^)it->Current;
                this->_lbviews->Items->Add(vd->GetName());
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
            this->InitializeButton(this->_btnDelete, 0, 1,
                    "Delete", gcnew EventHandler(this,
                    &ViewsMgrForm::OnDeleteClicked));
            this->InitializeButton(this->_btnSelectAll, 1, 1,
                    "Select All", gcnew EventHandler(this,
                    &ViewsMgrForm::OnSelectAllClicked));
            this->InitializeButton(this->_btnDeselectAll, 2, 1,
                    "Deselect All", gcnew EventHandler(this,
                    &ViewsMgrForm::OnDeselectAllClicked));
            this->InitializeButton(this->_btnNotOnSheets, 3, 1,
                    "Not On Sheet", gcnew EventHandler(this,
                    &ViewsMgrForm::OnNotOnSheetsClicked));
            this->InitializeButton(this->_btnLegends, 4, 1,
                    "Legends", gcnew EventHandler(this,
                    &ViewsMgrForm::OnLegendsClicked));                                                        
            this->InitializeButton(this->_btnCancel, 5, 1,
                    "Cancel", gcnew EventHandler(this,
                    &ViewsMgrForm::OnCancelClicked));                    
        }

        CL::IList   ^ViewsMgrForm::GetCheckedIndices(void)
        {
            return (this->_checkedIndices);
        }

        int         ViewsMgrForm::AreAllViewsChecked(void)
        {
            size_t  i;

            i = this->_lbviews->Items->Count;
            while (i--)
                if(!this->_lbviews->GetItemChecked(i))
                    return (0);
            return (1);
        }

        void    ViewsMgrForm::OnDeleteClicked(System::Object ^s, System::EventArgs ^e)
        {
            if (this->AreAllViewsChecked())
            {
                Forms::MessageBox::Show(
                    "Deleting all open views in a project is not allowed.",
                    "Warning");
                return ;
            }
            this->_checkedIndices = this->_lbviews->CheckedIndices;
            this->Close();
        }

        void    ViewsMgrForm::OnSelectAllClicked(System::Object ^s, System::EventArgs ^e)
        {
            size_t  i;

            i = this->_lbviews->Items->Count;
            while (i--)
                this->_lbviews->SetItemChecked(i, true);
        }

        void    ViewsMgrForm::OnDeselectAllClicked(System::Object ^s, System::EventArgs ^e)
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

        void    ViewsMgrForm::OnLegendsClicked(Object ^s, EventArgs ^e)
        {
            Forms::MessageBox::Show("Under Development");
        }

        void    ViewsMgrForm::OnCancelClicked(System::Object ^s, System::EventArgs ^e)
        {
            this->Close();
        }
    }
}