/*
 *  viewsmgr_form2.cpp
 */

 #include "viewsmgr_form2.h"
 #include "viewsmgr.h"

 using namespace Esta::Gui;
 using namespace Esta::ViewsMgr;

 MgrForm::MgrForm(IList ^viewData)
 {
    this->_selectedIds = gcnew Hashtable();
    this->_notOnSheet = gcnew ArrayList();
    InitializeComponent();
    InitializeTable();
    InitializeTreeView(viewData);
    AddButtons();
 }

 Hashtable   ^MgrForm::GetSelectedIds(void)
 {
     return this->_selectedIds;
 }

 void   MgrForm::InitializeComponent(void)
 {
    this->Text = L"Views Manager";
    this->FormBorderStyle = Forms::FormBorderStyle::Sizable;
    this->CenterToScreen();
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->MinimumSize = Dwg::Size(300,300);
 }

 void    MgrForm::AddButton(String ^text, EventHandler ^handler, int col, int row)
 {
    Button ^btn;

    btn = gcnew Button();
    btn->Text = text;
    btn->Click += handler;
    btn->Dock = Forms::DockStyle::Fill;
    this->_table->Controls->Add(btn, col, row);
 }

 void    MgrForm::AddButtons(void)
 {
    AddButton("Delete", 
        gcnew EventHandler(this, &MgrForm::OnDeleteClicked),
        1, 0);
    AddButton("Not On Sheet", 
        gcnew EventHandler(this, &MgrForm::OnNotOnSheetClicked),
        1, 1);
    AddButton("Cancel", 
        gcnew EventHandler(this, &MgrForm::OnCancelClicked),
        1, 2);
 }

 void   MgrForm::InitializeTable(void)
 {
    TableLayoutPanel   ^tab;
    Button              ^btn;

    tab = gcnew TableLayoutPanel();
    tab->Dock = Forms::DockStyle::Fill;
    tab->ColumnCount = 2;
    tab->RowCount = 4;
    tab->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Percent, 100));
    tab->ColumnStyles->Add(gcnew Forms::ColumnStyle(Forms::SizeType::Absolute, 100));
    tab->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
    tab->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
    tab->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
    tab->RowStyles->Add(gcnew Forms::RowStyle(Forms::SizeType::AutoSize));
    this->_table = tab;
    this->Controls->Add(this->_table);
 }

 void MgrForm::FillTreeView(IList ^viewData)
 {
     IEnumerator    ^it;
     ViewData       ^vd;
     ViewDataNode   ^top;
     ViewDataNode   ^nd;
     String         ^vt;
     bool           go;

     it = viewData->GetEnumerator();
     go = it->MoveNext();
     while (go)
     {
        vd = static_cast<ViewData ^>(it->Current);
        vt = vd->GetViewType().ToString();
        top = gcnew ViewDataNode(nullptr, vt, 0);
        nd = gcnew ViewDataNode(vd->GetUniqueId(),
            vd->GetName(), vd->IsOnSheet());
        top->Nodes->Add(nd);
        if (!nd->IsOnSheet)
            this->_notOnSheet->Add(nd);
        while ((go = it->MoveNext()) && String::Compare(
            static_cast<ViewData ^>(it->Current)->GetViewType().ToString(), vt) == 0)
        {
            vd = static_cast<ViewData ^>(it->Current);
            nd = gcnew ViewDataNode(vd->GetUniqueId(),
                vd->GetName(), vd->IsOnSheet());
            top->Nodes->Add(nd);
            if (!nd->IsOnSheet)
                this->_notOnSheet->Add(nd);
        }
        this->_tree->Nodes->Add(top);
     }
 }

 void   MgrForm::InitializeTreeView(IList ^viewData)
 {
    TreeView         ^tv;

    tv = gcnew TreeView();
    tv->Dock = Forms::DockStyle::Fill;
    tv->CheckBoxes = true;
    tv->AfterCheck += gcnew TreeViewEventHandler(this, &MgrForm::OnCheckBoxClicked);
    this->_tree = tv;
    this->FillTreeView(viewData);
    this->_table->SetRowSpan(this->_tree, this->_table->RowCount);
    this->_table->Controls->Add(this->_tree, 0, 0);
 }

 void    MgrForm::ToggleSelection(ViewDataNode ^nd, bool checked)
 {
    Hashtable   ^selIds;

    //nd->Checked = checked;
    selIds = this->_selectedIds;
    if (checked && !selIds->ContainsKey(nd->Id))
        selIds->Add(nd->Id, nd->Id);
    else
        selIds->Remove(nd->Id);
 }

 void    MgrForm::OnDeleteClicked(Object ^s, EventArgs ^e)
 {
   StringBuilder  ^bldr;
   IEnumerator    ^it;

   bldr = gcnew StringBuilder();
   it = this->_selectedIds->Keys->GetEnumerator();
   while (it->MoveNext())
   {
      bldr->AppendFormat("Id: {0}\n", (String ^)it->Current);
   }
   MessageBox::Show(bldr->ToString());
   this->Close();
 }

 void    MgrForm::OnCheckBoxClicked(Object ^s, TreeViewEventArgs ^e)
 {
    ViewDataNode  ^nd;
    IEnumerator   ^it;

   nd = static_cast<ViewDataNode ^>(e->Node);
   if (nd->Id == nullptr)
        CheckUncheckChildren(nd, nd->Checked);
   else
        ToggleSelection(nd, nd->Checked);
 }  

 void MgrForm::CheckUncheckChildren(ViewDataNode ^top, bool checked)
 { 
   IEnumerator      ^it;
   ViewDataNode     ^nd;

   it = top->Nodes->GetEnumerator();
   while (it->MoveNext())
       ((ViewDataNode ^)it->Current)->Checked = checked;
 }

 void    MgrForm::OnNotOnSheetClicked(Object^ s, EventArgs ^e)
 {
    StringBuilder  ^bldr;
    IEnumerator    ^it;
    ViewDataNode    ^nd;

    bldr = gcnew StringBuilder();
    it = this->_notOnSheet->GetEnumerator();
    while (it->MoveNext())
    {
        nd = (ViewDataNode ^)it->Current;
        bldr->AppendFormat("Name: {0}\n", nd->Name);
        nd->Checked = true;
    }
    MessageBox::Show(bldr->ToString());
 }

void    MgrForm::OnCancelClicked(Object ^s, EventArgs ^e)
{
    this->_selectedIds->Clear();
    this->Close();
}

 ViewDataNode::ViewDataNode(String ^id, String ^name, char isOnSheet)
 {
    this->Text = name;
    this->_name = name;
    this->_id = id;
    this->_isOnSheet = isOnSheet;
 }

 ViewDataNode::ViewDataNode(String ^name, array<TreeNode ^> ^children)
  : TreeNode(name, children)
 {
    this->_id = nullptr;
 }

 String  ^ViewDataNode::Id::get(void)
 {
    return (this->_id);
 }

 String ^ViewDataNode::Name::get(void)
 {
     return (this->_name);
 }

 char ViewDataNode::IsOnSheet::get(void)
 {
     return (this->_isOnSheet);
 }
