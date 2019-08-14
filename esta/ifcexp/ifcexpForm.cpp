/*
 *  ifcexpForm.cpp
 */

#include "ifcexpForm.h"

using namespace Esta;
using namespace Esta::Gui;

IfcExpForm::IfcExpForm(void)
{
    InitializeComponent();
}

void    IfcExpForm::InitializeComponent(void)
{
    this->Size = Dwg::Size(300, 300);
    this->Text = "IFC Batch Exporter";
}

void    IfcExpForm::InitializeTable(void)
{
    Forms::TableLayoutPanel ^tab;

    tab = gcnew Forms::TableLayoutPanel();
    tab->RowCount = 2;
    tab->ColumnCount = 2;
}

void    IfcExpForm::InitializeButton(String ^text,
            EventHandler ^handler, int col, int row)
{
    
}
