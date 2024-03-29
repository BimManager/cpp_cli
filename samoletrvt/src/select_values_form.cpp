// Copyright 2019 Samolet LLC
// Author: kkozlov
// select_values_form.cpp

#include "select_values_form.h"

namespace Samolet {
namespace Gui {
using Samolet::Helpers::SetOfValues;
SelectionForm::SelectionForm(array<SetOfValues^> ^sets) {
  Initialize(sets);
}

SelectionForm::SelectionForm(array<SetOfValues^> ^sets, bool check) {
    _suppress_check = check;
    Initialize(sets);
}

void   SelectionForm::InitializeComponent(void)
{
   this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
   this->Text = "Выбрать Значения";
   this->AutoSize = true;
   this->FormBorderStyle = Forms::FormBorderStyle::FixedSingle;
   this->AutoSizeMode = Forms::AutoSizeMode::GrowOnly;
   this->CenterToScreen();
   this->Load += gcnew EventHandler(this, &SelectionForm::OnLoadForm);
   this->MinimumSize = DWG::Size(300, 0);
}

void    SelectionForm::AddRow(String ^pn, array<String^> ^vals, int row, int defIndex)
{
   Forms::Label      ^lb;
   Forms::ComboBox   ^cbx;
   Forms::AutoCompleteStringCollection ^strCol;

   if (vals->Length < 2)
     return ;
   lb = gcnew Forms::Label();
   lb->AutoSize = true;
   lb->Text = pn;
   lb->Dock = Forms::DockStyle::Fill;
   lb->TextAlign = Dwg::ContentAlignment::MiddleLeft;
   lb->Margin = Forms::Padding(5);
   this->_panel->Controls->Add(lb, 0, row);
   cbx = gcnew Forms::ComboBox();
   cbx->AutoSize = true;
   cbx->Name = pn;
   cbx->Items->AddRange(vals);
   if (defIndex != -1)
     cbx->SelectedIndex = defIndex;
   cbx->AutoCompleteMode = Forms::AutoCompleteMode::SuggestAppend;
   cbx->DropDownStyle = Forms::ComboBoxStyle::DropDown;
   cbx->AutoCompleteSource = Forms::AutoCompleteSource::CustomSource;
   strCol = gcnew Forms::AutoCompleteStringCollection();
   strCol->AddRange(vals);
   cbx->AutoCompleteCustomSource = strCol;
   cbx->Dock = Forms::DockStyle::Fill;
   this->_panel->Controls->Add(cbx, 1, row);
}

void    SelectionForm::SetupOuterPanel(void)
{
   Forms::TableLayoutPanel  ^pnl;

   pnl = gcnew Forms::TableLayoutPanel();
   pnl->AutoSize = true;
   pnl->AutoSizeMode = Forms::AutoSizeMode::GrowOnly;
   pnl->Dock = Forms::DockStyle::Fill;
   pnl->ColumnCount = 2;
   pnl->RowCount = 2;
   pnl->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
   pnl->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
   this->Controls->Add(pnl);
   this->_outPanel = pnl;
}

void    SelectionForm::SetupPanel(void)
{
   Forms::TableLayoutPanel  ^pnl;

   pnl = gcnew Forms::TableLayoutPanel();
   pnl->AutoSize = true;
   pnl->AutoSizeMode = Forms::AutoSizeMode::GrowOnly;
   pnl->Dock = Forms::DockStyle::Fill;
   pnl->ColumnCount = 2;
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
   this->_outPanel->Controls->Add(pnl, 0, 0);
   this->_outPanel->SetColumnSpan(pnl, 2);
   this->_panel = pnl;
}

void  SelectionForm::SetupButtons(void)
{
   Forms::Button   ^btn;

   btn = gcnew Forms::Button();
   btn->Text = "OK";
   btn->Dock = Forms::DockStyle::Fill;
   btn->Click += gcnew EventHandler(this, &SelectionForm::OnOkClicked);
   btn->Margin = Forms::Padding(5);
   this->_outPanel->Controls->Add(btn, 0, 1);
   btn = gcnew Forms::Button();
   btn->Text = "Отмена";
   btn->Dock = Forms::DockStyle::Fill;
   btn->Margin = Forms::Padding(5);
   btn->Click += gcnew EventHandler(this, &SelectionForm::OnCancelClicked);
   this->_outPanel->Controls->Add(btn, 1, 1);
}

/*void    SelectionForm::PopulateView(array<ValuesSet^> ^sets)
{
   int               i;
   CL::IEnumerator   ^it;
   ValuesSet         ^set;
   size_t            nElems;
 
   this->_cFields = 0;
   it = sets->GetEnumerator();
   nElems = ValuesSet::GetElemCount();
   while (it->MoveNext())
   {
      set = dynamic_cast<ValuesSet^>(it->Current);
      if (set->GetCount() && set->GetCount() == nElems)
         this->AddRow(set->GetName(), set->GetValues(), this->_cFields++);
   }
   }*/

void    SelectionForm::PopulateView(array<SetOfValues^> ^sets)
{
   int               i;
   CL::IEnumerator   ^it;
   SetOfValues         ^set;
   int		          nElems;
   int			nParams;
   
   _cFields = 0;
   it = sets->GetEnumerator();
   nElems = SetOfValues::GetSelectedElemsCount();
   while (it->MoveNext()) {
     set = dynamic_cast<SetOfValues^>(it->Current);
     nParams = set->GetRvtParams()->Count;
     LOG(String::Format("n_params {0}; n_elems: {1}", nParams, nElems));
     if (_suppress_check) {
       LOG("suppressed");
       AddRow(set->GetParamName(), set->GetValuesAsString(), _cFields++,
              Array::IndexOf(set->GetValuesAsString(), set->GetDefaultValue()));
     } else {
       if (nParams && nParams >= nElems)
         AddRow(set->GetParamName(), set->GetValuesAsString(), _cFields++,
                Array::IndexOf(set->GetValuesAsString(), set->GetDefaultValue()));
     }
   }
}

void	SelectionForm::FillSelected(void)
{
   CL::IEnumerator   ^it;
   Forms::ComboBox   ^cbx;

   it = this->_panel->Controls->GetEnumerator();
   while (it->MoveNext())
   {
      if ((cbx = dynamic_cast<Forms::ComboBox^>(it->Current)) != nullptr)
         this->_selected->Add(cbx->Name, dynamic_cast<String^>(cbx->SelectedItem));
   }
}

CL::Hashtable^ SelectionForm::GetSelected(void) {
   return (this->_selected);
}

void SelectionForm::OnLoadForm(Object ^s, EventArgs ^e) {
  if (this->_cFields == 0) {
      Forms::MessageBox::Show("Параметры не найдены.");
      this->DialogResult = WINFORMS::DialogResult::Cancel;
      //      this->Close();
  }
}

void    SelectionForm::OnOkClicked(Object ^s, EventArgs ^e) {
   s = nullptr;
   e = nullptr;
   FillSelected();
   this->Close();
}

void    SelectionForm::OnCancelClicked(Object ^s, EventArgs ^e)
{
   s = nullptr;
   e = nullptr;
   this->_selected->Clear();
   this->Close();
}
}  // namespace gui
}  // namespace Samolet
