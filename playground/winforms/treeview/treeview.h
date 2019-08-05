/*                                  
 * treeview.h    
 */

#ifndef TREE_VIEW_H
# define TREE_VIEW_H

# using <System.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

namespace Forms = System::Windows::Forms;
namespace Dwg = System::Drawing;

using System::Object;
using System::String;
using System::EventArgs;
using System::EventHandler;

using System::Text::StringBuilder;

using System::Windows::Forms::Application;
using System::Windows::Forms::Form;
using System::Windows::Forms::TableLayoutPanel;
using System::Windows::Forms::TreeView;
using System::Windows::Forms::TreeViewEventHandler;
using System::Windows::Forms::TreeViewEventArgs ;

using System::Windows::Forms::Button;
using System::Windows::Forms::TreeNode;
using System::Windows::Forms::MessageBox;

using System::Collections::IEnumerator;
using System::Collections::Hashtable;

ref class ViewDataNode : public TreeNode
{
public:
    ViewDataNode(String ^id, String ^name, char isOnSheet);
    ViewDataNode(String ^name, array<ViewDataNode ^> ^children);
    property String ^Id
    {
        String  ^get();
    }
private:
    String  ^_id;
    String  ^_name;
    char    _isOnSheet;
};


public ref class MgrTreeView : Form
{
public:
    MgrTreeView(void);
private:
    TableLayoutPanel    ^_table;
    TreeView            ^_tree;
    Hashtable           ^_selected;

    void    InitializeComponent(void);
    void    InitializeTable(void);
    void    InitializeTreeView(void);

    static void ModifyChildren(ViewDataNode ^top, bool checked);

    void    OnClickBtn(Object ^s, EventArgs ^e);
    void    OnCheckBox(Object ^s, TreeViewEventArgs ^e);
};

#endif