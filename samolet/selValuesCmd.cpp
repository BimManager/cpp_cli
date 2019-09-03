/*
 *  selValuesCmd.cpp
 */

#include "selValuesCmd.h" 
#include "selValuesCmdForm.h"
#include "utils.h"

using namespace Samolet;
using namespace Samolet::Commands;

ValuesSet::ValuesSet(String ^name, array<String^> ^values)
{
    this->_name = name;
    this->_values = values;
    this->_params = gcnew CLG::LinkedList<DB::Parameter^>();
    this->_paramsCount = 0;
}

String  ^ValuesSet::GetName(void)
{
    return (this->_name);
}

array<String^>  ^ValuesSet::GetValues(void)
{
    return (this->_values);
}

CLG::LinkedList<DB::Parameter^>   ^ValuesSet::GetParameters(void)
{
    return (this->_params);
}

size_t ValuesSet::GetCount(void)
{
    return (this->_paramsCount);
}

void    ValuesSet::AddParameter(DB::Parameter ^p)
{
    this->_paramsCount++;
    this->_params->AddLast(p);
}

void    ValuesSet::SetElemCount(size_t n)
{
    s_totalParams = n;
}

int ValuesSet::Compare(Object ^o1, Object ^o2)
{
    ValuesSet   ^set1;
    ValuesSet   ^set2;

    set1 = dynamic_cast<ValuesSet^>(o1);
    set2 = dynamic_cast<ValuesSet^>(o2);
    if (set1 == nullptr || set2 == nullptr)
        return (-1);
    return (set1->GetName()->CompareTo(set2->GetName()));
}

int ValuesSet::CompareTo(Object ^other)
{
    ValuesSet   ^otherSet;

    otherSet = dynamic_cast<ValuesSet^>(other);
    if (otherSet == nullptr)
        return (-1);
    return (this->GetName()->CompareTo(otherSet->GetName()));
}

bool    AvailableUponSelection::IsCommandAvailable(
            UI::UIApplication ^uiapp, DB::CategorySet ^selCats)
{
    selCats = nullptr;
    if (uiapp->ActiveUIDocument->Selection->GetElementIds()->Count)
        return (true);
    return (false);
}            

String      ^SetSpCmd::LocateParamsSource(void)
{
    REF::Assembly   ^asmb;
    String          ^filePath;
    IO::FileInfo    ^fi;
    
    asmb = REF::Assembly::GetExecutingAssembly();
    /*filePath = String::Format("{0}\\{1}", asmb->Location,
                asmb->GetName()->Name->Replace(".dll", ".txt"));*/
    if (Utils::ConfigFileManager::
            GetValueFromConfigFile("filePath", filePath))
    {
    #ifdef DEBUG
        Debug::WriteLine(String::Format(
            "Cannot get the value from the config file."));
    #endif
        return (nullptr);
    }
    #ifdef DEBUG
        Debug::WriteLine(String::Format("filePath: {0}",
            filePath));
    #endif
    fi = gcnew IO::FileInfo(filePath);
    if (!fi->Exists)
        return (nullptr);
    return (filePath);
}

array<ValuesSet^>   ^SetSpCmd::FillValuesSets(String ^filePath)
{
    CL::Hashtable       ^pvs;
    array<ValuesSet^>   ^sets;
    CL::IEnumerator     ^it;
    ValuesSet           ^cs;
    String              ^pn;
    int                 i;

    pvs = ParamsSourceParser::ParseTxtFile(filePath);
    sets = gcnew array<ValuesSet^>(pvs->Keys->Count);
    it = pvs->Keys->GetEnumerator();
    i = 0;
    while (it->MoveNext())
    {
        pn = dynamic_cast<String ^>(it->Current);
        cs = gcnew ValuesSet(pn, dynamic_cast<array<String^>^>(pvs[pn]));
    #ifdef DEBUG
        Debug::WriteLine(String::Format("[RVT] PN: {0}; Count: {1}.",
            pn, cs->GetValues()->Length));
    #endif
        sets[i++] = cs;
    }
    System::Array::Sort(sets);
    return (sets);
}

void    SetSpCmd::ExtractParameters(UI::UIDocument ^uidoc,
            array<ValuesSet^> ^sets)
{
    CL::IEnumerator         ^idIt;
    CL::IEnumerator         ^setIt;
    DB::Parameter           ^p;
    DB::Document            ^doc;
    ValuesSet               ^set;

    doc = uidoc->Document;
    ValuesSet::SetElemCount((size_t)uidoc->Selection->GetElementIds()->Count);
    idIt = uidoc->Selection->GetElementIds()->GetEnumerator();
    setIt = sets->GetEnumerator();
    while (setIt->MoveNext())
    {
        set = dynamic_cast<ValuesSet^>(setIt->Current);
        while (idIt->MoveNext())
        {
            p = doc->GetElement(dynamic_cast<DB::ElementId^>(idIt->Current))
                ->LookupParameter(set->GetName());
            if (p != nullptr)
            {
            #ifdef DEBUG
                Debug::WriteLine(String::Format("[RVT] Actual Param:{0}, Name:{1}",
                    p->Definition->Name, set->GetName()));
            #endif
                set->AddParameter(p);
            }
        }
        idIt->Reset();
    }
    #ifdef DEBUG
    setIt->Reset();
    while (setIt->MoveNext())
    {
        set = dynamic_cast<ValuesSet^>(setIt->Current);
        Debug::WriteLine(String::Format("[RVT] Name:{0}  Values Count: {1} Params: {2}, {3}",
            set->GetName(), set->GetValues()->Length, 
            set->GetParameters()->Count, set->GetCount()));
    }
    #endif
}

void    SetSpCmd::SetValues(DB::Document ^doc, 
            CL::Hashtable ^selected, array<ValuesSet^> ^sets)
{
    DB::Transaction ^tr;
    CL::IEnumerator ^setIt;
    CL::IEnumerator ^pIt;
    ValuesSet       ^set;
    String          ^val;

    if (!selected->Count)
        return ;
    setIt = sets->GetEnumerator();
    tr = gcnew DB::Transaction(doc);
    tr->Start("Set values");
    while (setIt->MoveNext())
    {
        set = dynamic_cast<ValuesSet^>(setIt->Current);
        if (set->GetCount())
        {
            val = dynamic_cast<String^>(selected[set->GetName()]);
            if (!val)
                continue ;
            pIt = set->GetParameters()->GetEnumerator();
            while (pIt->MoveNext())
                Utils::RvtParamManager::SetStringValue(
                    dynamic_cast<DB::Parameter^>(pIt->Current), val);
        }
    }
    tr->Commit();
}

CL::Hashtable   ^ParamsSourceParser::ParseTxtFile(String ^filePath)
{
   CL::Hashtable         ^pmtsVals;
   IO::StreamReader  ^sr;
   String            ^line;
   array<String^>    ^parts;
   array<String^>    ^vals;

   pmtsVals = gcnew CL::Hashtable();
   sr = gcnew IO::StreamReader(filePath, System::Text::Encoding::Unicode);
   try
   {
      while ((line = sr->ReadLine()))
      {
         if (!line->Length)
            continue ;
         parts = line->Split('\t');
         if (parts->Length != 2)
            continue;
         vals = parts[1]->Split(';');
         if (!pmtsVals->Contains(parts[0]))
            pmtsVals->Add(parts[0], vals);
      }
   }
   finally
   {
     sr->Close();
   }
   return (pmtsVals);
}

UI::Result  SetSpCmd::CarryOutCmd(UI::ExternalCommandData ^data, 
                String ^%msg, DB::ElementSet ^elements)
{
    UI::UIDocument      ^uidoc;
    DB::Document        ^doc;
    Gui::SelectionForm  ^form;
    String              ^filePath;
    array<ValuesSet^>   ^sets;
    CL::Hashtable       ^ht;

    uidoc = data->Application->ActiveUIDocument;
    doc = uidoc->Document;
#ifdef DEBUG
    Debug::WriteLine(String::Format("[RVT] Title: {0}",
        doc->Title));
#endif
    uidoc = data->Application->ActiveUIDocument;
    filePath = this->LocateParamsSource();
    sets = this->FillValuesSets(filePath);
    this->ExtractParameters(uidoc, sets);
    form = gcnew Gui::SelectionForm(sets);
    form->ShowDialog();
    ht = form->GetSelected();
    #ifdef DEBUG
    CL::IEnumerator ^it;
    String          ^key;
    it = ht->Keys->GetEnumerator();
    while (it->MoveNext())
    {
        key = dynamic_cast<String^>(it->Current);
        Debug::WriteLine(String::Format("[RVT] {0} : {1}",
            key, dynamic_cast<String^>(ht[key])));
    }
    #endif
    SetValues(doc, ht, sets);
    return (UI::Result::Succeeded);
}

UI::Result  SetSpCmd::Execute(UI::ExternalCommandData ^data, 
                String ^%msg, DB::ElementSet ^elements)
{    
    return (CarryOutCmd(data, msg, elements));
}