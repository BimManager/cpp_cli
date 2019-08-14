/*
 *  pullout.cpp
 */

#include "pullout.h"

namespace Rvt
{
    Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
    {
        DB::Document                ^doc;

        doc = cmdData->Application->ActiveUIDocument->Document;
        PullOutCategories(doc);
        return (Autodesk::Revit::UI::Result::Succeeded);
    }

    int Command::PullOutCategories(DB::Document ^doc)
    {
        StreamWriter                ^sw;
        TXT::StringBuilder          ^bldr;
        DB::CategoryNameMapIterator ^it;
        DB::Categories              ^cats;
        DB::Category                ^c;

        cats = doc->Settings->Categories;
        bldr = gcnew TXT::StringBuilder();
        it = cats->ForwardIterator();
        while (it->MoveNext())
        {
            c = dynamic_cast<DB::Category ^>(it->Current);
            bldr->AppendLine(String::Format("{0}\t{1}",
                    c->Name, c->Id->ToString()));
        }
        sw = gcnew StreamWriter(String::Format("{0}\\{1}", 
                System::Environment::GetFolderPath(
                    System::Environment::SpecialFolder::MyDocuments),
                IO::Path::GetRandomFileName()), false, TXT::Encoding::Unicode);
        sw->Write(bldr->ToString());
        sw->Close();
        return (0);
    }
}