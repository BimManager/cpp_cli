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

# define ACTION_IMPORT_FROM_FILE    0
# define ACTION_IMPORT_FROM_MODEL   1
# define ACTION_EXPORT_TO_FILE      2
# define ACTION_EXPORT_TO_MODEL     3

namespace Esta
{
    using Autodesk::Revit::DB::ModelPath;
    using System::String;

    namespace EventData
    {
        public ref class DismissedDialogEventArgs : public System::EventArgs
        {
        public:
            DismissedDialogEventArgs(String ^filepath, ModelPath ^modelPath, char action);
            ModelPath       ^GetModelPath(void);
            String          ^GetFilepath(void);
            char            GetAction(void);
        private:
            String          ^_filepath;
            ModelPath       ^_modelPath;
            char            _action;
        };
    }

    namespace Gui
    {
        namespace Forms = System::Windows::Forms;
        namespace DWG = System::Drawing;
        using Forms::DialogResult;

        using Autodesk::Revit::UI::FileOpenDialog;
        using Autodesk::Revit::UI::ItemSelectionDialogResult;
        using Autodesk::Revit::DB::ModelPathUtils;

        public delegate void VoidDelegateVoid(void);

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
            String                  ^_filepath;
            ModelPath               ^_modelPath;

            void    InitializeComponent(void);
            void    InitializeTableLayout(Forms::TableLayoutPanel ^%tab);
            void    InitializeButton(String ^title,
                        int col, int row, System::EventHandler ^handler);
            void    InitializeTextBox(void);
            void    InitializeButtons(void);
            void    BringUpSaveDialog(void);
            void    BringUpOpenDialog(void);
            void    ExportImport(char action1, char action2, VoidDelegateVoid ^pfn);
            void    HandleExportImport(char action);

            void    OnSelectModelClicked(System::Object ^s, System::EventArgs ^e);
            void    OnImportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnExportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
        };
    }
}

#endif
