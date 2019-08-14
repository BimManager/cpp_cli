/*
 *  ifcexpForm.h
 */

 #ifndef IFC_EXP_FORM_H
 # define IFC_EXP_FORM_H

 # using <System.dll>
 # using <System.Windows.Forms.dll>
 # using <System.Drawing.dll>

 namespace Esta
 {
    namespace Gui
    {
        namespace Sys = System;
        using Sys::String;
        using Sys::Object;
        using Sys::EventHandler;
        namespace Forms = System::Windows::Forms;
        namespace Dwg = System::Drawing;
        

        public ref class IfcExpForm : Forms::Form
        {
        public:
            IfcExpForm(void);
        private:
            Forms::TableLayoutPanel ^_table;

            void    InitializeComponent(void);
            void    InitializeTable(void);
            void    InitializeButton(String ^text,
                        EventHandler ^handler, int col, int row);
        };
    }
 }

 #endif