/*
 *  nwd_tmpl.cpp
 */

#include "nwd_props.h"

namespace Tmpl
{
    int NwdTmpl::Execute( ... array<String ^> ^parameters)
    {
        Api::Document                       ^doc;
        DocParts::DocumentCurrentSelection  ^sel;
        Api::ModelItemCollection            ^items;
        Api::ModelItem                      ^item;
        StrBuilder                          ^strBld;
        Api::PropertyCategoryCollection     ^props;
        Api::DataProperty                   ^pr;
        
        doc = Api::Application::ActiveDocument;
        sel = doc->CurrentSelection;
        if (!sel->IsEmpty)
        {
            strBld = gcnew StrBuilder();
            items = sel->SelectedItems;
            item = items->First;
            props = item->PropertyCategories;
            pr = props->FindPropertyByName(
                Api::PropertyCategoryNames::Item,
                Api::DataPropertyNames::ItemName);
            strBld->AppendLine(String::Format("{0} : {1}", pr->Name, pr->Value->ToString()));
            System::Windows::Forms::MessageBox::Show(
            Api::Application::Gui->MainWindow, strBld->ToString());
        }

        System::Windows::Forms::MessageBox::Show(
            Api::Application::Gui->MainWindow, "Sweet Fanny Adams");
        return (0);
    }
}
