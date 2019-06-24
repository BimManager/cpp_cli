/*
 *  form1.h
 */
 
#ifndef FORM1_H
# define FORM1_H

//# using <System.ComponentModel.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>
# using <System.dll>

namespace Esta
{
    namespace Gui
    {
        namespace Forms = System::Windows::Forms;
        namespace DWR = System::Drawing;

        public ref class FilePicker : public Forms::Form
        {
        public:
            FilePicker(void);
        //protected:
            // ~FilePicker(void);
        private:
            Forms::Button    ^_btnSelFile;

            void InitializeComponent(void);
            void BtnOnClicked(System::Object ^s, System::EventArgs ^e);
        };
    }

}

#endif
