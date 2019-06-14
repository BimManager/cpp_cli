/*
 *  classification.cpp
 */

#include "classification.h"

#define CATEGORY_ID -2000011
#define PARAMETER_NAME "Assembly Code"
#define PARAMETER_VALUE "42.42.42.42"

namespace Classification
{
    Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                UI::UIDocument          ^uidoc;
                Selection::Selection    ^sel;
                Classifier              ^cls;
                ICollection<DB::ElementId ^>    ^ids;
                IEnumerator<DB::ElementId ^>    ^itr;  

                uidoc = cmdData->Application->ActiveUIDocument;
                cls = gcnew Classifier(uidoc->Document);
                sel = uidoc->Selection;
                ids = cls->BagsyElementIds((Autodesk::Revit::DB::BuiltInCategory)CATEGORY_ID);
                sel->SetElementIds(ids);
                if (ids->Count > 0)
                {
                    itr = ids->GetEnumerator();
                    while (itr->MoveNext())
                        cls->SetParameter(uidoc->Document->GetElement(itr->Current)->LookupParameter(PARAMETER_NAME),
                        PARAMETER_VALUE);
                }
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

    ICollection<DB::ElementId ^> ^Classifier::BagsyElementIds(DB::BuiltInCategory category)
    {
        DB::FilteredElementCollector    ^col;
        DB::ElementCategoryFilter       ^catFltr;

        col = gcnew DB::FilteredElementCollector(this->_doc);
        catFltr = gcnew DB::ElementCategoryFilter(category);
        return (col->WherePasses(catFltr)->WhereElementIsElementType()->ToElementIds());
    }

    void Classifier::PrintParameter(DB::Element ^elem, System::String ^name)
    {    
        DB::Parameter   ^param;

        param = elem->LookupParameter(name);
        if (param != nullptr)
            UI::TaskDialog::Show("Info", param->Definition->Name + ": " 
                + this->ParameterAsString(param));

    }

    int Classifier::SetParameter(DB::Parameter ^parameter, System::Object ^value)
    {
        DB::Transaction ^trn;

        trn = gcnew DB::Transaction(this->_doc);
        trn->Start("Set a value to a parameter");
        switch (parameter->StorageType)
        {
            case DB::StorageType::String:
                parameter->Set((System::String ^)value);
                break;
            case DB::StorageType::Integer:
                parameter->Set((System::Int32)value);
                break;
            case DB::StorageType::Double:
                parameter->Set((System::Double)value);
                break;
            case DB::StorageType::ElementId:
                parameter->Set((DB::ElementId ^)value);
                break;
            case DB::StorageType::None:
                trn->RollBack();
                return (1);
            default:
                trn->RollBack();
                return (2);
        }   
        trn->Commit();
        return (0);
    }

    System::String ^Classifier::ParameterAsString(DB::Parameter ^parameter)
    {
        switch (parameter->StorageType)
        {
            case DB::StorageType::String:
                return (parameter->AsString());
            case DB::StorageType::Integer:
                return (parameter->AsInteger().ToString());
            case DB::StorageType::Double:
                return (parameter->AsDouble().ToString());
            case DB::StorageType::ElementId:
                return (parameter->AsElementId()->ToString());
            case DB::StorageType::None:
                return ("None");
            default:
                return ("N/A");
        }   
    }

}