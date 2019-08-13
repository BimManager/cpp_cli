/*
 *  dataGridView.h
 */

#ifndef DATA_GRID_VIEW_H
# define DATA_GRID_VIEW_H

# using <System.dll>
# using <System.Windows.Forms.dll>
# using <System.Drawing.dll>

using System::Object;
using System::EventHandler;
using System::EventArgs;

using System::Windows::Forms::Application;
using System::Windows::Forms::Form;
using System::Windows::Forms::DataGridView;
using System::Windows::Forms::TableLayoutPanel;
using System::Windows::Forms::Panel;
using System::Windows::Forms::Button;
using System::Windows::Forms::DockStyle;

namespace Dwg = System::Drawing;

using System::Drawing::Size;
using System::Drawing::Point;

public ref class Form1 : Form
{
public:
    Form1(void);

private:
    Panel           ^_btnPanel;
    DataGridView    ^_songsGridView;
    Button          ^_addBtn;
    Button          ^_delBtn;

    void            SetupLayout(void);
    void            SetupDataGridView(void);
    //void            PopulateDataGridView(void);

    //void            OnForm1Load(Object ^s, EventArgs ^e);
    void            OnAddClicked(Object ^s, EventArgs ^e);
    void            OnDeleteClicked(Object ^s, EventArgs ^e);

};

#endif

