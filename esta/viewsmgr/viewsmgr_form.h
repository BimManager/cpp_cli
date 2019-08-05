/*
 *  viewsmgr_form.h
 */

#ifndef VIEWS_MGR_FORM_H
# define VIEWS_MGR_FORM_H

# using <System.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

#include "viewsmgr.h"

namespace Esta
{
    namespace Gui
    {
        namespace Forms = System::Windows::Forms;
        namespace DWG = System::Drawing;
        namespace CL = System::Collections;

        using namespace Esta::ViewsMgr;

        typedef System::String          String;
        typedef System::Object          Object;
        typedef System::EventArgs       EventArgs;
        typedef System::EventHandler    EventHandler;

        public ref class   ViewsMgrForm : Forms::Form
        {
        public:
            /* ViewsMgrForm(void); */
            ViewsMgrForm(CL::IList ^names, CL::IList ^viewData);

            CL::IList   ^GetCheckedIndices(void);
        protected:
            /* ~ViewsMgrForm(void) == Dispose(void) */
            /* !ViewsMgrForm(void) == Finalize(void) */
        private:
            Forms::TableLayoutPanel ^_layout;
            Forms::CheckedListBox   ^_lbviews;
            Forms::Button           ^_btnDelete;
            Forms::Button           ^_btnSelectAll;
            Forms::Button           ^_btnDeselectAll;
            Forms::Button           ^_btnNotOnSheets;
            Forms::Button           ^_btnLegends;
            Forms::Button           ^_btnCancel;
            CL::IList               ^_viewNames;
            CL::IList               ^_checkedIndices;
            CL::IList               ^_viewData;

            void    InitializeComponent(void);
            void    InitializeComponent(CL::IList ^viewData);
            void    InitializeTableLayoutPanel(void);
            void    InitializeListBox(CL::IList ^viewNames);
            void    InitializeButtons(void);
            void    InitializeButton(Forms::Button ^%btn, 
                    int row, int col,
                    String ^text, System::EventHandler ^handler);

            void    FillListBox(void);    
            void    FillListBoxVD(void);
            int     AreAllViewsChecked(void);                

            void    OnDeleteClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
            void    OnSelectAllClicked(System::Object ^s, System::EventArgs ^e);
            void    OnNotOnSheetsClicked(System::Object ^s, System::EventArgs ^e);
            void    OnDeselectAllClicked(System::Object ^s, System::EventArgs ^e);
            void    OnLegendsClicked(Object ^s, EventArgs ^e);
        };
    }
}


#endif