/*
 *  viewsmgr_form.h
 */

#ifndef VIEWS_MGR_FORM_H
# define VIEWS_MGR_FORM_H

# using <System.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

namespace Esta
{
    namespace Gui
    {
        namespace Forms = System::Windows::Forms;
        namespace DWG = System::Drawing;
        namespace CL = System::Collections;

        typedef System::String          String;
        typedef System::Object          Object;
        typedef System::EventArgs       EventArgs;
        typedef System::EventHandler    EventHandler;

        public ref class   ViewsMgrForm : Forms::Form
        {
        public:
            ViewsMgrForm(void);
            ViewsMgrForm(CL::IList ^names);

            CL::IList   ^GetCheckedItems(void);
        protected:
            /* ~ViewsMgrForm(void) == Dispose(void) */
            /* !ViewsMgrForm(void) == Finalize(void) */
        private:
            Forms::TableLayoutPanel ^_layout;
            Forms::CheckedListBox   ^_lbviews;
            Forms::Button           ^_btnDelete;
            Forms::Button           ^_btnSelectAll;
            Forms::Button           ^_btnDiselectAll;
            Forms::Button           ^_btnNotOnSheets;
            Forms::Button           ^_btnCancel;
            CL::IList               ^_viewNames;
            CL::IList               ^_checkedIndices;

            void    InitializeComponent(void);
            void    InitializeTableLayoutPanel(void);
            void    InitializeListBox(CL::IList ^viewNames);
            void    InitializeButtons(void);
            void    InitializeButton(Forms::Button ^%btn, 
                    int row, int col,
                    String ^text, System::EventHandler ^handler);

            void    FillListBox(void);                    

            void    OnDeleteClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
            void    OnSelectAllClicked(System::Object ^s, System::EventArgs ^e);
            void    OnNotOnSheetsClicked(System::Object ^s, System::EventArgs ^e);
            void    OnDiselectAllClicked(System::Object ^s, System::EventArgs ^e);
        };
    }
}


#endif