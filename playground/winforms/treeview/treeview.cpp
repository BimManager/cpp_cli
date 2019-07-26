/*
 *  treeview.cpp
 */

 #include "treeview.h"

 MgrTreeView::MgrTreeView(void)
 {
    InitializeComponent();
    InitializeTable();
    InitializeTreeView();
    //MessageBox::Show(this->_table->ToString());
 }

 void   MgrTreeView::InitializeComponent(void)
 {
    this->Text = L"MgrTreeView";
    this->Height = 300;
    this->Width = 300;
    this->CenterToScreen();
 }

 void   MgrTreeView::InitializeTable(void)
 {
    TableLayoutPanel   ^tab;
    Button              ^btn;

    tab = gcnew TableLayoutPanel();
    tab = gcnew TableLayoutPanel();
    tab->Dock = Forms::DockStyle::Fill;
    tab->BackColor = Dwg::Color::Red;
    tab->ColumnCount = 2;
    tab->RowCount = 1;
    tab->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
    tab->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 50));
    tab->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::Percent, 100));
    btn = gcnew Button();
    btn->Text = "Click";
    btn->Click += gcnew EventHandler(this, &MgrTreeView::OnClickBtn);
    tab->Controls->Add(btn, 1, 0);
    this->_table = tab;
    this->Controls->Add(this->_table);
 }

 void   MgrTreeView::InitializeTreeView(void)
 {
    TreeView    ^tv;
    TreeNode    ^tn;
    array<TreeNode ^> ^nds;

    nds = gcnew array<TreeNode ^>(3);
    nds[0] = gcnew TreeNode("Foo");
    nds[1] = gcnew TreeNode("Bar");
    nds[2] = gcnew TreeNode("Baz");
    tn = gcnew TreeNode("Top-level", nds);
    tv = gcnew TreeView();
    tv->Nodes->Add(tn);
    tv->Dock = Forms::DockStyle::Fill;
    tv->CheckBoxes = true;
    this->_tree = tv;
    this->_table->Controls->Add(this->_tree, 0, 0);
 }

 void    MgrTreeView::OnClickBtn(Object ^s, EventArgs ^e)
 {
     TreeNode       ^top;
     StringBuilder  ^bld;

     top = this->_tree->TopNode;
     MessageBox::Show(sel->Text);
 }