/*                                  
 * setParamsCmdForm.h
 */

#ifndef SET_PARAMS_CMD_FORM
# define SET_PARAMS_CMD_FORM

# include "stdafx.h"
# include "setSpCmd.h"

namespace Samolet
{
    namespace Gui
    {
        using Samolet::Commands::ValuesSet;

        public ref class SelectionForm : Forms::Form
        {
        public:
            SelectionForm(CL::Hashtable^ prmsVals);
            SelectionForm(array<ValuesSet^> ^sets);
            CL::Hashtable   ^GetSelected(void);
        private:
            CL::Hashtable           ^_pmtsVals;
            CL::Hashtable           ^_selected;
            Forms::TableLayoutPanel    ^_outPanel;
            Forms::TableLayoutPanel    ^_panel;

            void    InitializeComponent(void);
            void    SetupOuterPanel(void);
            void    SetupPanel(void);
            void    SetupButtons(void);
            void    PopulateView(void);
            void    PopulateView(array<ValuesSet^> ^sets);

            void    AddRow(String ^pn, array<String^> ^vals, int row);
            void    PopulateSelected(void);
            void    FillSelected(void);

            void    OnOkClicked(Object ^s, EventArgs ^e);
            void    OnCancelClicked(Object ^s, EventArgs ^e);
        };
    }
}

#endif