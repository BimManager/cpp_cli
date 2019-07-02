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

        typedef System::String          String;
        typedef System::Object          Object;
        typedef System::EventArgs       EventArgs;
        typedef System::EventHandler    EventHandler;

        public ref class   ViewsMgrForm : Forms::Form
        {
        public:
            ViewsMgrForm(void);
        protected:
            /* ~ViewsMgrForm(void) == Dispose(void) */
            /* !ViewsMgrForm(void) == Finalize(void) */
        private:
            Forms::TableLayoutPanel ^_layout;
            Forms::CheckedListBox   ^_lbviews;
            Forms::Button           ^_btnDelete;
            Forms::Button           ^_btnCancel;

            void    InitializeComponent(void);
            void    InitializeTableLayoutPanel(void);
            void    InitializeListBox(void);
            void    InitializeButtons(void);
            void    InitializeButton(Forms::Button ^%btn, 
                    int row, int col,
                    String ^text, System::EventHandler ^handler);

            void    OnDeleteClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
        };
    }
}


#endif