// Copyright 2019 Samolet LLC
// Author: kkozlov
// family_params_cmd.cpp

#include "family_params_cmd.h"

#include "family_params_form.h"
#include "serialization_utils.h"

namespace Samolet {
namespace Commands {
ref class PreserveValuesLoadFamily : IFamilyLoadOptions {
 public:
  virtual bool	OnFamilyFound(bool familyInUse,
                              bool %overwriteParameterValues) {
    if (familyInUse)
      overwriteParameterValues = false;
    return (true);
  }
  
  virtual bool	OnSharedFamilyFound(Family ^sharedFamily,
                                    bool familyInUse,
                                    FamilySource %source,
                                    bool %overwriteParameterValues) {
    if (familyInUse) {
      source = FamilySource::Family;
      overwriteParameterValues = false;
    }
    return (true);
  }
};

ICollection<Element^> ^CollectFamilySymbols(Document ^doc) {
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

void	LoadFamilyBack(Document ^doc, Document ^famDoc) {
  famDoc->LoadFamily(doc, gcnew PreserveValuesLoadFamily());
}


void	EditFamilyDoc(Document ^doc, Family ^family,
                      FamilyParamDefinition ^paramDef,
                      ParameterModifierHandler ^pFunc) {
  Document	^famDoc;
  DB::Transaction	^tx;
  
  if (!family->IsEditable)
    return ;
  famDoc = doc->EditFamily(family);
#ifdef DEBUG
  //  Debug::WriteLine(String::Format("famDoc title: {0}",
  //                                    famDoc->Title));
#endif
  tx = gcnew DB::Transaction(famDoc);
  tx->Start("Edit family");
  pFunc(famDoc->FamilyManager, paramDef);
  tx->Commit();
  LoadFamilyBack(doc, famDoc);
  famDoc->Close(false);
}

void	RemoveParameter(FamilyManager ^famMgr, FamilyParamDefinition ^def) {
  IEnumerator<FamilyParameter^>	^fpIt;
  FamilyParameter		^fp;

  fpIt = famMgr->GetParameters()->GetEnumerator();
  while (fpIt->MoveNext()) {
    if (static_cast<DB::InternalDefinition^>(
            (fp = fpIt->Current)->Definition)->BuiltInParameter
        != DB::BuiltInParameter::INVALID)
      continue ;
    if (String::Compare(fp->Definition->Name,
                        def->Name) == 0
        && fp->Definition->ParameterType
        == def->ParameterType
        && fp->Definition->ParameterGroup
        == def->ParameterGroup &&
        fp->IsInstance == def->IsInstance)
    {
      famMgr->RemoveParameter(fp);
#ifdef DEBUG
      Debug::WriteLine(String::Format(
          "{0} in {1} has been removed",
          def->Name, famMgr->CurrentType->Name));
#endif
      
    }
  }

}

UI::Result FamilyParameterRemoverCmd::Execute(
    UI::ExternalCommandData ^cmdData,
    String ^%msg,
    DB::ElementSet ^elements) {
  using Samolet::Gui::ParamsControllerForm;
  namespace utils = samoletrvt::utils;
  namespace io = System::IO;
  typedef System::Environment env;
    Document			^doc;
    IEnumerator<Element^>	^famIt;
    ParamsControllerForm^	form;
    FamilyParamDefinition^	def;
     
    doc = cmdData->Application->ActiveUIDocument->Document;
    famIt = CollectFamilySymbols(doc)->GetEnumerator();
    form = gcnew ParamsControllerForm();
    form->ShowDialog();
    if ((def = form->GetParamDefinition()) != nullptr)
      while (famIt->MoveNext())
        EditFamilyDoc(doc, static_cast<FamilySymbol^>(famIt->Current)->Family,
                      def,
                      gcnew ParameterModifierHandler(&RemoveParameter));
    return (UI::Result::Succeeded);
}
} // namespace Commands
} // namespace Samolet
