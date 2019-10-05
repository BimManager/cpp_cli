/*
 *  rvttmp.cpp
 */

#include "cmdtmp.h"

using namespace Firm;
using namespace Firm::Commands;

int  Firm::GetValueFromConfigFile(String ^key, String ^%out)
{
    Configuration   ^config;
    int             ret;

    config = ConfigurationManager::OpenExeConfiguration(
            Assembly::GetExecutingAssembly()->Location);
    if (config->HasFile)
    {  
        ret = config->AppSettings->Settings[key] != nullptr ? 0 : 1;
        if (!ret) 
            out = config->AppSettings->Settings[key]->Value;
    }
    else
        ret = 2;
    return (ret);
}

Object  ^Firm::GetValueFromConfigFile(String ^key, int %err)
{
    Configuration   ^config;

    config = ConfigurationManager::OpenExeConfiguration(
            Assembly::GetExecutingAssembly()->Location);
    if (config->HasFile)
    {  
        err = config->AppSettings->Settings[key] != nullptr ? 0 : 1;
        if (!err) 
            return ((String ^)config->AppSettings->Settings[key]->Value);
    }
    else
        err = 2;
    return (nullptr);
}

using System::Collections::Generic::ICollection;
using System::Collections::Generic::IEnumerator;
using Autodesk::Revit::DB::Element;
using Autodesk::Revit::DB::Document;
using Autodesk::Revit::DB::FilteredElementCollector;
using Autodesk::Revit::DB::FamilySymbol;
using Autodesk::Revit::DB::Family;
using Autodesk::Revit::DB::IFamilyLoadOptions;
using Autodesk::Revit::DB::FamilySource;
using Autodesk::Revit::DB::FamilyManager;
using Autodesk::Revit::DB::FamilyParameter;
using Autodesk::Revit::DB::ParameterType;
using Autodesk::Revit::DB::BuiltInParameterGroup;
using Autodesk::Revit::DB::Definition;
namespace DB = Autodesk::Revit::DB;

//#define LOGDBG(msg) #ifdef DEBUG Debug::WriteLine(String::Format(msg)) #endif

ref class PreserveValuesLoadFamily : IFamilyLoadOptions
{
 public:
  virtual bool	OnFamilyFound(bool familyInUse,
                              bool %overwriteParameterValues)
  {
    if (familyInUse)
      overwriteParameterValues = false;
    return (true);
  }
  
  virtual bool	OnSharedFamilyFound(Family ^sharedFamily,
                                    bool familyInUse,
                                    FamilySource %source,
                                    bool %overwriteParameterValues)
  {
    if (familyInUse)
    {
      source = FamilySource::Family;
      overwriteParameterValues = false;
    }
    return (true);
  }
};

ref class FamilyParamDefinition
{
 public:
  FamilyParamDefinition() {}
  FamilyParamDefinition(String ^name, DB::ParameterType type,
                        BuiltInParameterGroup group,
                        bool isInstance)
      : _name{name}, _paramType{type}, _paramGroup{group},
        _isInstance{isInstance} {}
  virtual property String^ Name {
    String^ get(void) {
      return (_name);
    }
  }
  virtual property DB::ParameterType ParameterType {
    DB::ParameterType get(void) {
      return (_paramType);
    }
  }
  virtual property BuiltInParameterGroup ParameterGroup {
    BuiltInParameterGroup get(void) {
      return (_paramGroup);
    }
  }
  virtual property bool Instance {
    bool get(void)  {
      return (_isInstance);
    }
  }
 private:
  String^		 	_name;
  DB::ParameterType		_paramType;
  BuiltInParameterGroup		_paramGroup;
  bool				_isInstance;
};

delegate void ParameterModifierHandler(FamilyManager ^famMgr,
                                FamilyParamDefinition ^paramDef);


ICollection<Element^> ^CollectFamilySymbols(Document ^doc)
{
  FilteredElementCollector	^col;
  ICollection<Element^>		^famSymbls;

  col = gcnew FilteredElementCollector(doc);
  famSymbls = col->OfClass(FamilySymbol::typeid)
      ->WhereElementIsElementType()
      ->ToElements();
  #ifdef DEBUG
  Debug::WriteLine(String::Format(
      "Number of elems: {0};",
      famSymbls->Count));
  #endif
  return (famSymbls);
}

void	LoadFamilyBack(Document ^doc, Document ^famDoc)
{
  famDoc->LoadFamily(doc, gcnew PreserveValuesLoadFamily());
}


void	EditFamilyDoc(Document ^doc, Family ^family,
                      FamilyParamDefinition ^paramDef,
                      ParameterModifierHandler ^pFunc)
{
  Document	^famDoc;
  DB::Transaction	^tx;
  
  if (!family->IsEditable)
    return ;
  famDoc = doc->EditFamily(family);
#ifdef DEBUG
  Debug::WriteLine(String::Format("famDoc title: {0}",
                                        famDoc->Title));
#endif
  tx = gcnew DB::Transaction(famDoc);
  tx->Start("Edit family");
  pFunc(famDoc->FamilyManager, paramDef);
  tx->Commit();
  LoadFamilyBack(doc, famDoc);
  famDoc->Close(false);
}

void	RemoveParameter(FamilyManager ^famMgr, FamilyParamDefinition ^def)                        
{
  IEnumerator<FamilyParameter^>	^fpIt;
  FamilyParameter		^fp;

  fpIt = famMgr->GetParameters()->GetEnumerator();
  while (fpIt->MoveNext())
  {
    if ((fp = fpIt->Current)->IsReadOnly)
      continue ;
    if (String::Compare(fp->Definition->Name,
                        def->Name) == 0
        && fp->Definition->ParameterType
        == def->ParameterType
        && fp->Definition->ParameterGroup
        == def->ParameterGroup)
      famMgr->RemoveParameter(fp);
  }

}

Autodesk::Revit::UI::Result Command::Execute(
    Autodesk::Revit::UI::ExternalCommandData ^cmdData,
    System::String ^%msg,
    Autodesk::Revit::DB::ElementSet ^elements)
{
#ifdef DEBUG
    Debug::WriteLine(String::Format("{0} has started.",
        Assembly::GetExecutingAssembly()->FullName));
#endif
    Document			^doc;
    IEnumerator<Element^>	^famIt;

    doc = cmdData->Application->ActiveUIDocument->Document;
    famIt = CollectFamilySymbols(doc)->GetEnumerator();
    while (famIt->MoveNext())
      EditFamilyDoc(doc, static_cast<FamilySymbol^>(famIt->Current)->Family,
                    gcnew FamilyParamDefinition("Offset",
                                                DB::ParameterType::Length,
                                                BuiltInParameterGroup::PG_GEOMETRY,
                                                false), 
                    gcnew ParameterModifierHandler(&RemoveParameter));
    return (Autodesk::Revit::UI::Result::Succeeded);
}
