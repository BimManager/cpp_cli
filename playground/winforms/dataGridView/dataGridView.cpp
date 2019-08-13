/*
 *  dataGridView.cpp
 */

#include "dataGridView.h"
 
Form1::Form1(void)
{
    this->_btnPanel = gcnew Panel();
    this->_songsGridView = gcnew DataGridView();
    this->_addBtn = gcnew Button();
    this->_delBtn = gcnew Button();
}

void    Form1::SetupLayout(void)
{
    this->Size = Dwg::Size(500, 600);

    this->_addBtn->Text = "Add Row";
    this->_addBtn->Location = Point(10, 10);
    this->_addBtn->Click += gcnew EventHandler(this, &Form1::OnAddClicked);

    this->_delBtn->Text = "Delete Row";
    this->_delBtn->Location = Point(100, 10);
    this->_delBtn->Click += gcnew EventHandler(this, &Form1::OnDeleteClicked);

    this->_btnPanel->Controls->Add(this->_addBtn);
    this->_btnPanel->Controls->Add(this->_delBtn);
    this->_btnPanel->Height = 50;
    this->_btnPanel->Dock = DockStyle::Bottom;

    this->Controls->Add(this->_btnPanel);
}

void    Form1::SetupDataGridView(void)
{
    this->Controls->Add(this->_songsGridView);

    this->_songsGridView->ColumnCount = 5;

    this->_songsGridView->Name = "songsGridView";
    this->_songsGridView->Location = Point(8, 8);
    this->_songsGridView->Size = Dwg::Size(500, 250);
}

void            Form1::OnAddClicked(Object ^s, EventArgs ^e)
{

}

void            Form1::OnDeleteClicked(Object ^s, EventArgs ^e)
{

}
