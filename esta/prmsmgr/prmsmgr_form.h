/*
 *  prmsmgr_form.h
 */
 
#ifndef FORM1_H
# define FORM1_H

//# using <System.ComponentModel.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>
# using <System.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

# define ACTION_IMPORT  0
# define ACTION_EXPORT  1

namespace Esta
{
    namespace EventData
    {
        public ref class DismissedDialogEventArgs : public System::EventArgs
        {
        public:
            DismissedDialogEventArgs(System::String ^filepath, char action);
            System::String  ^GetFilepath(void);
            char            GetAction(void);
        private:
            System::String  ^_filepath;
            char            _action;
        };
    }

    namespace Gui
    {
        namespace Forms = System::Windows::Forms;
        namespace DWG = System::Drawing;
        using Autodesk::Revit::UI::FileOpenDialog;
        using Autodesk::Revit::UI::ItemSelectionDialogResult;
        using Autodesk::Revit::DB::ModelPath;
        using Autodesk::Revit::DB::ModelPathUtils;

        public ref class FilePicker : public Forms::Form
        {
        public:
            FilePicker(void);
            event System::EventHandler  ^DialogDismissed;
        protected:
            virtual void OnDialogDismissed(EventData::DismissedDialogEventArgs ^e);
        private:
            Forms::TableLayoutPanel ^_tab;
            Forms::TextBox          ^_txtBox;
            System::String          ^_filepath;
            System::String          ^_modelpath;
            

            void    InitializeComponent(void);
            void    InitializeTableLayout(Forms::TableLayoutPanel ^%tab);
            void    InitializeButton(System::String ^title,
                        int col, int row, System::EventHandler ^handler);
            void    InitializeTextBox(void);
            void    InitializeButtons(void);
            void    BringUpSaveDialog(void);
            void    BringUpOpenDialog(void);

            void    OnSelectModelClicked(System::Object ^s, System::EventArgs ^e);
            void    OnImportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnExportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
        };
    }
}

#endif
