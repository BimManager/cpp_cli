/*
 *  selValuesCmdForm.cpp
 */

#include "selValuesCmdForm.h"

using namespace Samolet;
using namespace Samolet::Gui;

SelectionForm::SelectionForm(CL::Hashtable ^pmtsVals)
{
   this->_pmtsVals = pmtsVals;
   this->_selected = gcnew CL::Hashtable();
   //InitializeComponent();
   SetupOuterPanel();
   SetupPanel();
   PopulateView();
}

SelectionForm::SelectionForm(array<ValuesSet^> ^sets)
{
   this->_selected = gcnew CL::Hashtable();
   InitializeComponent();
   SetupOuterPanel();
   SetupPanel();
   PopulateView(sets);
   SetupButtons();
}

void   SelectionForm::InitializeComponent(void)
{
   this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
   this->Text = "Select Values";
   this->Size = Dwg::Size(300, 35);
   this->MinimumSize = Dwg::Size(300, 0);
   //this->AutoSizeMode = Forms::AutoSizeMode::GrowAndShrink;
   this->CenterToScreen();
}

void    SelectionForm::AddRow(String ^pn, array<String^> ^vals, int row)
{
   Forms::Label      ^lb;
   Forms::ComboBox   ^cbx;
   Forms::AutoCompleteStringCollection ^strCol;

   lb = gcnew Forms::Label();
   lb->Text = pn;
   lb->Dock = Forms::DockStyle::Fill;
   this->_panel->Controls->Add(lb, 0, row);
   cbx = gcnew Forms::ComboBox();
   cbx->Name = pn;
   cbx->Items->AddRange(vals);
   cbx->SelectedIndex = 0;
   cbx->AutoCompleteMode = Forms::AutoCompleteMode::SuggestAppend;
   cbx->DropDownStyle = Forms::ComboBoxStyle::DropDown;
   cbx->AutoCompleteSource = Forms::AutoCompleteSource::CustomSource;
   strCol = gcnew Forms::AutoCompleteStringCollection();
   strCol->AddRange(vals);
   cbx->AutoCompleteCustomSource = strCol;
   cbx->Dock = Forms::DockStyle::Fill;
   this->_panel->Controls->Add(cbx, 1, row);
   this->Height += cbx->Height + 15;
}

void    SelectionForm::SetupOuterPanel(void)
{
   Forms::TableLayoutPanel  ^pnl;

   pnl = gcnew Forms::TableLayoutPanel();
   pnl->Dock = Forms::DockStyle::Fill;
   pnl->ColumnCount = 2;
   pnl->RowCount = 3;
   pnl->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
   pnl->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::Percent, 100));
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::Absolute, 35));
   //pnl->RowStyles->Add(gcnew RowStyle(Forms::SizeType::AutoSize));
   this->Controls->Add(pnl);
   this->_outPanel = pnl;
}

void    SelectionForm::SetupPanel(void)
{
   Forms::TableLayoutPanel  ^pnl;

   pnl = gcnew Forms::TableLayoutPanel();
   pnl->Dock = Forms::DockStyle::Fill;
   pnl->ColumnCount = 2;
   //pnl->ColumnStyles->Add(gcnew ColumnStyle(Forms::SizeType::Percent, 50));
   //pnl->ColumnStyles->Add(gcnew ColumnStyle(Forms::SizeType::Percent, 50));
   pnl->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
   this->_outPanel->Controls->Add(pnl, 0, 0);
   this->_outPanel->SetColumnSpan(pnl, 2);
   this->_panel = pnl;
}

void  SelectionForm::SetupButtons(void)
{
   Forms::Button   ^btn;

   btn = gcnew Forms::Button();
   btn->Text = "Ok";
   btn->Dock = Forms::DockStyle::Fill;
   btn->Click += gcnew EventHandler(this, &SelectionForm::OnOkClicked);
   this->_outPanel->Controls->Add(btn, 0, 1);
   btn = gcnew Forms::Button();
   btn->Text = "Cancel";
   btn->Dock = Forms::DockStyle::Fill;
   btn->Click += gcnew EventHandler(this, &SelectionForm::OnCancelClicked);
   this->_outPanel->Controls->Add(btn, 1, 1);
}

void    SelectionForm::PopulateView(void)
{
   CL::IEnumerator    ^it;
   String             ^pn;
   Forms::Label       ^lb;
   Forms::ComboBox    ^cbx;
   int                i;

   it = this->_pmtsVals->Keys->GetEnumerator();
   i = 0;
   while (it->MoveNext())
   {
      pn = dynamic_cast<String^>(it->Current);
      lb = gcnew Forms::Label();
      lb->Text = pn;
      lb->Dock = Forms::DockStyle::Fill;
      this->_panel->Controls->Add(lb, 0, i);
      cbx = gcnew Forms::ComboBox();
      cbx->Name = pn;
      cbx->Items->AddRange(dynamic_cast<array<String^>^>(this->_pmtsVals[pn]));
      cbx->SelectedIndex = 0;
      cbx->DropDownStyle = Forms::ComboBoxStyle::DropDownList;
      cbx->Dock = Forms::DockStyle::Fill;
      this->_panel->Controls->Add(cbx, 1, i++);
   }
   SetupButtons();
}

void    SelectionForm::PopulateView(array<ValuesSet^> ^sets)
{
   int               i;
   CL::IEnumerator   ^it;
   ValuesSet         ^set;

   i = 0;
   it = sets->GetEnumerator();
   while (it->MoveNext())
   {
      set = dynamic_cast<ValuesSet^>(it->Current);
      this->AddRow(set->GetName(), set->GetValues(), i++);
   }
}

void  SelectionForm::PopulateSelected(void)
{
   CL::IEnumerator   ^it;
   Forms::ComboBox   ^cbx;
   String            ^pn;
   String            ^val;

   it = this->_pmtsVals->Keys->GetEnumerator();
   this->_selected = gcnew CL::Hashtable();
   while (it->MoveNext())
   {
      pn = dynamic_cast<String^>(it->Current);
      cbx = dynamic_cast<Forms::ComboBox^>(this->_panel->Controls[pn]);
      val = dynamic_cast<String^>(cbx->SelectedItem);
      System::Console::WriteLine(
         String::Format("{0} - {1}", pn, val));
      this->_selected->Add(pn, val);
   }
}

void    SelectionForm::FillSelected(void)
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

CL::Hashtable   ^SelectionForm::GetSelected(void)
{
   return (this->_selected);
}

void    SelectionForm::OnOkClicked(Object ^s, EventArgs ^e)
{
   //PopulateSelected();
   FillSelected();
   this->Close();
}

void    SelectionForm::OnCancelClicked(Object ^s, EventArgs ^e)
{
   this->_selected->Clear();
   this->Close();
}