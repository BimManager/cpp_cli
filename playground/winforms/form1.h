/*
 *  form1.h
 */

#ifndef FORM1_H
# define FORM1_H

# include "stdafx.h"

namespace Forms
{
    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void);
    protected:        
        ~Form1(void);
    private:
        System::ComponentModel::Container   ^_components;
        System::Windows::Forms::Label       ^_label;

        void    InitializeComponent(void);
        void    LabelClick(System::Object ^s, System::EventArgs ^e);
    };
}

#endif
