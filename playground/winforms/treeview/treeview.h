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

using System::Windows::Forms::Button;
using System::Windows::Forms::TreeNode;
using System::Windows::Forms::MessageBox;


public ref class MgrTreeView : Form
{
public:
    MgrTreeView(void);
private:
    TableLayoutPanel    ^_table;
    TreeView            ^_tree;

    void    InitializeComponent(void);
    void    InitializeTable(void);
    void    InitializeTreeView(void);

    void    OnClickBtn(Object ^s, EventArgs ^e);
};

#endif