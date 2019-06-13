/*
 *  classification.cpp
 */

#include "classification.h"

namespace Classification
{
    Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                UI::UIDocument          ^uidoc;
                Selection::Selection    ^sel;
                Classifier ^cls;

                uidoc = cmdData->Application->ActiveUIDocument;
                cls = gcnew Classifier(uidoc->Document);
                sel = uidoc->Selection;
                sel->SetElementIds(cls->BagsyElements(DB::BuiltInCategory::OST_Walls));
                return (Autodesk::Revit::UI::Result::Succeeded);
            }
    Classifier::Classifier(DB::Document ^doc)     
    {
        this->_doc = doc;
    }
    Classifier::~Classifier()
    {
        this->!Classifier();
        UI::TaskDialog::Show("Destructor", "The class has been destroyed.");
    }
    Classifier::!Classifier()
    {
        UI::TaskDialog::Show("Finalizer","Done!");
    }

    ICollection<DB::ElementId ^> ^Classifier::BagsyElements(DB::BuiltInCategory category)
    {
        DB::FilteredElementCollector    ^col;
        DB::ElementCategoryFilter       ^catFltr;

        col = gcnew DB::FilteredElementCollector(this->_doc);
        catFltr = gcnew DB::ElementCategoryFilter(category);
        return (col->WherePasses(catFltr)->WhereElementIsNotElementType()->ToElementIds());
    }
}