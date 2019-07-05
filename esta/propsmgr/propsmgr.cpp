/*
 *  propsmgr.cpp
 */

#include "propsmgr.h"

namespace Esta
{
    int PropsMgr::Execute( ... array<String ^> ^parameters)
    {
        Api::Document   ^doc;
        Api::Model      ^model;
        Api::ModelItem  ^rootItem;
        Api::ModelGeometry  ^geom;
        StringBuilder   ^info;

        doc = Api::Application::ActiveDocument;
        model = doc->Models[0];
        rootItem = model->RootItem;
        if (rootItem != nullptr)
        {
            PropsMgr::PrintProperties(rootItem);
        }
        return (0);
    }

    void    PropsMgr::PrintProperties(Api::ModelItem ^item)
    {
        Api::PropertyCategoryCollection ^catCol;
        Api::PropertyCategory           ^cat;
        Api::DataPropertyCollection     ^propCol;
        Api::DataProperty               ^prop;

        catCol = item->PropertyCategories;
        cat = catCol->FindCategoryByName(
            Api::PropertyCategoryNames::Item);
        propCol = cat->Properties;
        prop = propCol->FindPropertyByName(Api::DataPropertyNames::ItemName);
        Forms::MessageBox::Show(Api::Application::Gui->MainWindow,
            String::Format("{0} : {1}", prop->Name, prop->Value));
    }
}
