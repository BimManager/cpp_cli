/*
 *  classification.h
 *  To classify building elements 
 *  in accordance with UniClass
 */

#ifndef ESTA_CLASSIFICATION
# define ESTA_CLASSIFICATION

//# using <mscorlib.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

using namespace System::Collections::Generic;
namespace DB = Autodesk::Revit::DB;
namespace UI = Autodesk::Revit::UI;
namespace Selection = Autodesk::Revit::UI::Selection;

namespace Classification
{
    [Autodesk::Revit::Attributes::TransactionAttribute(
        Autodesk::Revit::Attributes::TransactionMode::Manual)]
    public ref class Command : Autodesk::Revit::UI::IExternalCommand
    {
    public:
        virtual Autodesk::Revit::UI::Result Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements);
    };

    ref class Classifier
    {
    public:
        Classifier(DB::Document ^doc);
        ~Classifier(void); /* IDisposable.Dispose() */ 
        !Classifier(void); /* Finalize() */
        ICollection<DB::ElementId ^> ^BagsyElements(DB::BuiltInCategory category);
    private:
        DB::Document ^_doc;
    };
}

#endif