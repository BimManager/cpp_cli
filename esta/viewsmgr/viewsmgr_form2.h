/*                                  
 *  viewsmgr_form2.h
 */

#ifndef VIEWS_MGR_FORM_2_H
# define VIEWS_MGR_FORM_2_H

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
using System::Collections::SortedList;
using System::Collections::IList;
using System::Collections::ICollection;
using System::Collections::ArrayList;

namespace Esta
{
    namespace Gui
    {
        ref class ViewDataNode : public TreeNode
        {
        public:
            ViewDataNode(String ^id, String ^name, char isOnSheet);
            ViewDataNode(String ^name, array<TreeNode ^> ^children);
            property String ^Id
            {
                String  ^get(void);
            }
            property String ^Name
            {
                String  ^get(void);
            }
            property char   IsOnSheet
            {
                char    get(void);
            }
        private:
            String  ^_id;
            String  ^_name;
            char    _isOnSheet;
        };


        public ref class MgrForm : Form
        {
        public:
            MgrForm(IList ^viewData);
            Hashtable   ^GetSelectedIds(void);

        private:
            TableLayoutPanel    ^_table;
            TreeView            ^_tree;
            Hashtable           ^_selectedIds;
            IList               ^_notOnSheet;

            void    InitializeComponent(void);
            void    InitializeTable(void);
            void    InitializeTreeView(IList ^viewData);

            void    FillTreeView(IList ^viewData);
            void    CheckUncheckChildren(ViewDataNode ^top, bool checked);
            void    ToggleSelection(ViewDataNode ^nd, bool checked);

            void    AddButton(String ^text, EventHandler ^handler, int col, int row);
            void    AddButtons(void);

            void    OnCheckBoxClicked(Object ^s, TreeViewEventArgs ^e);
            void    OnDeleteClicked(Object ^s, EventArgs ^e);
            void    OnNotOnSheetClicked(Object^ s, EventArgs ^e);
            void    OnCancelClicked(Object ^s, EventArgs ^e);
        };
    }
}

#endif