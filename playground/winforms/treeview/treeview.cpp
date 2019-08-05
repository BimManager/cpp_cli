/*
 *  treeview.cpp
 */

 #include "treeview.h"

 MgrTreeView::MgrTreeView(void)
 {
    InitializeComponent();
    InitializeTable();
    InitializeTreeView();
    this->_selected = gcnew Hashtable();
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
    TreeView         ^tv;
    ViewDataNode    ^tn;
    array<ViewDataNode ^>  ^vnds;

    /*nds = gcnew array<TreeNode ^>(3);
    nds[0] = gcnew TreeNode("Foo");
    nds[1] = gcnew TreeNode("Bar");
    nds[2] = gcnew TreeNode("Baz");
    tn = gcnew TreeNode("Top-level", nds);
    tv = gcnew TreeView();
    tv->Nodes->Add(tn); */
    vnds = gcnew array<ViewDataNode ^>(4);
    vnds[0] = gcnew ViewDataNode("42", "Node_42", 1);
    vnds[1] = gcnew ViewDataNode("123", "Node_123", 1);
    vnds[2] = gcnew ViewDataNode("xxx", "Node_xxx", 1);
    vnds[3] = gcnew ViewDataNode("abcd", "Node_abcd", 1);
    tn = gcnew ViewDataNode("Top", vnds);
    tv = gcnew TreeView();
    tv->Dock = Forms::DockStyle::Fill;
    tv->CheckBoxes = true;
    tv->Nodes->Add(tn);
    tv->AfterCheck += gcnew TreeViewEventHandler(this, &MgrTreeView::OnCheckBox);
    this->_tree = tv;
    this->_table->Controls->Add(this->_tree, 0, 0);
 }

 void    MgrTreeView::OnClickBtn(Object ^s, EventArgs ^e)
 {
   StringBuilder  ^bldr;
   IEnumerator    ^it;

   bldr = gcnew StringBuilder();
   it = this->_selected->Keys->GetEnumerator();
   while (it->MoveNext())
   {
      bldr->AppendFormat("Id: {0}\n", (String ^)it->Current);
   }
   MessageBox::Show(bldr->ToString());
 }

 void    MgrTreeView::OnCheckBox(Object ^s, TreeViewEventArgs ^e)
 {
    ViewDataNode  ^nd;
    IEnumerator   ^it;

   nd = static_cast<ViewDataNode ^>(e->Node);
   if (nd->Id == nullptr)
      ModifyChildren(nd, nd->Checked);
   else
   {
      if (nd->Checked)
         this->_selected->Add(nd->Id, nd->Id);
      else
         this->_selected->Remove(nd->Id);
   }

      /*/MessageBox::Show(String::Format("{0} {1} has been {2}; Count: {3}",
      vdnd->Text,
      vdnd->Id,
      vdnd->Checked ? "checked" : "unchecked",
      vdnd->Nodes->Count));
      if (vdnd->Nodes->Count)
      {
         it = vdnd->Nodes->GetEnumerator();
         while (it->MoveNext())
         {
            ((ViewDataNode ^)it->Current)->Checked = vdnd->Checked;
         }
      }*/
 }

 void MgrTreeView::ModifyChildren(ViewDataNode ^top, bool checked)
 {
   IEnumerator   ^it;

   it = top->Nodes->GetEnumerator();
   while (it->MoveNext())
      ((ViewDataNode ^)it->Current)->Checked = checked;
 }

 ViewDataNode::ViewDataNode(String ^id, String ^name, char isOnSheet)
 {
    this->Text = name;
    this->_id = id;
    this->_isOnSheet = isOnSheet;
 }

 ViewDataNode::ViewDataNode(String ^name, array<ViewDataNode ^> ^children)
  : TreeNode(name, children)
 {
    this->_id = nullptr;
 }

 String  ^ViewDataNode::Id::get()
 {
    return this->_id;
 }