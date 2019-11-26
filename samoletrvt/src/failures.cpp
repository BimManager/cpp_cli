// Copyright 2019 Samolet LLC
// Author: kkozlov
// failures.cpp

#include "failures.h"
#include "utils.h"

namespace Samolet {
namespace FailuresHandling {
ElementEditingInsideGroup::ElementEditingInsideGroup(DB::Document ^doc) {
    this->_doc = doc;
}

DB::FailureProcessingResult
ElementEditingInsideGroup::PreprocessFailures(DB::FailuresAccessor ^fa)
{
    CL::IEnumerator             ^it;
    DB::FailureMessageAccessor  ^fma;
    DB::FailureDefinitionId     ^fId;

    #ifdef DEBUG
    #endif
    it = fa->GetFailureMessages()->GetEnumerator();
    fa->JournalFailures(fa->GetFailureMessages());
    while (it->MoveNext())
    {
        fma = dynamic_cast<DB::FailureMessageAccessor^>(it->Current);
        fId = fma->GetFailureDefinitionId();
        if (fma->GetSeverity() == DB::FailureSeverity::Error)
        {
            if (fId == DB::BuiltInFailures::GroupFailures
                        ::AtomViolationWhenMultiPlacedInstances)
            {
                #ifdef DEBUG
                Debug::WriteLine("WhenMultiPlacedInstances");
                Debug::WriteLine(String::Format(
                    "Caption: {0}; ResNums: {1}; ResType: {2}",
                    fma->GetDefaultResolutionCaption(), 
                    fma->GetNumberOfResolutions(),
                    fma->GetCurrentResolutionType()
                ));
                Debug::WriteLine(String::Format("FailingIds: {0}", 
                    fma->GetFailingElementIds()->Count));
                #endif
                this->FillFailingData(fa->GetDocument(), fma->GetFailingElementIds());
                fa->ResolveFailure(fma);
                return (DB::FailureProcessingResult::ProceedWithCommit);
            }
        }
        else if (fma->GetSeverity() == DB::FailureSeverity::Warning)
        {
            if (fId == DB::BuiltInFailures::GroupFailures
                        ::AtomViolationWhenOnePlaceInstance)
            {
                fa->DeleteWarning(fma);
                #ifdef DEBUG
                Debug::WriteLine("Warning has been deleted");
                #endif
            }
        }
    }
    //return (DB::FailureProcessingResult::ProceedWithRollBack);
    return (DB::FailureProcessingResult::Continue);
}

CLG::ICollection<DB::ElementId^>^
ElementEditingInsideGroup::GetFailingIds(void)
{
    return (this->_failingIds);
}

String^
ElementEditingInsideGroup::GetGroupName(void)
{
    return (this->_groupName);
}

void    ElementEditingInsideGroup::
FillFailingData(DB::Document ^doc, 
    CLG::ICollection<DB::ElementId^> ^failIds)
{
    CL::IEnumerator ^it;
    DB::Group       ^grp;

    #ifdef DEBUG
    Debug::WriteLine("Inside FillFailingData");
    Debug::WriteLine(String::Format("Count: {0}",
        failIds->Count));
    #endif
    it = failIds->GetEnumerator();
    while (it->MoveNext())
    {        
        grp = dynamic_cast<DB::Group^>(
            doc->GetElement(
                dynamic_cast<DB::ElementId^>(it->Current)));
        if (grp != nullptr)
        {
            this->_failingIds = grp->GetMemberIds();
            this->_groupName = grp->Name;            
            #ifdef  DEBUG
            Debug::WriteLine("group is not nullptr");
            Debug::WriteLine(String::Format("Name:{0}; Id Count: {1}",
                this->_groupName, this->_failingIds->Count));
            #endif
            break ;
        }
    }
}

DB::ElementId^
ElementEditingInsideGroup::FindGroupType(DB::Document ^doc, String ^groupName)
{
    DB::FilteredElementCollector    ^col;
    DB::ElementParameterFilter      ^nameFltr;

    nameFltr = gcnew DB::ElementParameterFilter(
        DB::ParameterFilterRuleFactory::CreateEqualsRule(
        gcnew DB::ElementId(
            DB::BuiltInParameter::SYMBOL_NAME_PARAM), groupName, false));
    col = gcnew DB::FilteredElementCollector(doc);
    return (col->OfClass(DB::GroupType::typeid)
            ->WherePasses(nameFltr)
            ->FirstElementId());
}

CLG::ICollection<DB::Element^>^
ElementEditingInsideGroup::FindGroupsByTypeId(DB::Document ^doc, DB::ElementId ^id)
{
    DB::FilteredElementCollector    ^col;
    DB::ElementParameterFilter      ^idFltr;

    idFltr = gcnew DB::ElementParameterFilter(
        DB::ParameterFilterRuleFactory::CreateEqualsRule(
        gcnew DB::ElementId(DB::BuiltInParameter::ELEM_TYPE_PARAM), id));
    col = gcnew DB::FilteredElementCollector(doc);
    return (col->OfClass(DB::Group::typeid)
            ->WherePasses(idFltr)
            ->WhereElementIsNotElementType()
            ->ToElements());
}

void    ElementEditingInsideGroup::ProcessFailedGroups(void)
{
    #ifdef DEBUG
    DB::ElementId                   ^grpTypeId;
    CLG::ICollection<DB::Element^>^ grps;

    grpTypeId = this->FindGroupType(this->_doc, this->_groupName);
    if (grpTypeId != nullptr)
    {
        Debug::WriteLine(
            String::Format("Group with id: {0} has been located.",
            grpTypeId->ToString()));
        //grps = this->FindGroupsByTypeId(this->_doc, grpTypeId);
        grps = Samolet::Utils::RvtFilters::FindElemsOfClassByTypeId(
            this->_doc, DB::Group::typeid, grpTypeId);
        Debug::WriteLine(
            String::Format("{0} groups have been located.",
                grps->Count));
    }
    #endif
}

void FailuresUtils::SetFailuresPreprocessor(
        DB::Transaction ^tr, DB::IFailuresPreprocessor ^fp)
{
    DB::FailureHandlingOptions  ^failOpts;

    failOpts = tr->GetFailureHandlingOptions();
    failOpts->SetFailuresPreprocessor(fp);
    failOpts->SetForcedModalHandling(false);
    tr->SetFailureHandlingOptions(failOpts);
}
}  // namespace FailuresHandling
}  // namespace Samolet
