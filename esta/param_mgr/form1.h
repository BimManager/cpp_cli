/*
 *  form1.h
 */
 
#ifndef FORM1_H
# define FORM1_H

//# using <System.ComponentModel.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>
# using <System.dll>

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

        public ref class FilePicker : public Forms::Form
        {
        public:
            FilePicker(void);
            event System::EventHandler  ^DialogDismissed;
        protected:
            virtual void OnDialogDismissed(EventData::DismissedDialogEventArgs ^e);
        private:
            Forms::Button   ^_btnPickFile;
            Forms::Button   ^_btnImport;
            Forms::Button   ^_btnExport;
            Forms::Button   ^_btnCancel;
            Forms::Label    ^_lblFilename;

            void    InitializeComponent(void);
            void    InitializeButton(Forms::Button ^btn, System::String ^title,
                        DWG::Point pos, System::EventHandler ^handler);
            void    InitializeButtons(void);
            void    InitializeLabel(Forms::Label ^lbl);

            void    OnPickClicked(System::Object ^s, System::EventArgs ^e);
            void    OnImportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnExportClicked(System::Object ^s, System::EventArgs ^e);
            void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
        };
    }
}

#endif
