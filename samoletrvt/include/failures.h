/*
 *  failuresHandling.h
 */

#ifndef FAILURES_HANDLING_H
# define FAILURES_HANDLING_H

# include "stdafx.h"

namespace Samolet
{
    namespace FailuresHandling
    {
        public ref class ElementEditingInsideGroup : DB::IFailuresPreprocessor
        {
        public:
            ElementEditingInsideGroup(DB::Document ^doc);
            virtual DB::FailureProcessingResult
            PreprocessFailures(DB::FailuresAccessor ^fa);
            CLG::ICollection<DB::ElementId^>    
            ^GetFailingIds(void);
            String  
            ^GetGroupName(void);
            void    
            ProcessFailedGroups(void);
        private:
            DB::Document    ^_doc;
            CLG::ICollection<DB::ElementId^> ^_failingIds;
            String  ^_groupName;

            void
            FillFailingData(DB::Document ^doc, 
                CLG::ICollection<DB::ElementId^> ^failIds);
            DB::ElementId^
            FindGroupType(DB::Document ^doc, String ^groupName);
            CLG::ICollection<DB::Element^>^
            FindGroupsByTypeId(DB::Document ^doc, DB::ElementId ^id);
        };

        public ref class FailuresUtils
        {
        public:
            static void
            SetFailuresPreprocessor(DB::Transaction ^tr,
                DB::IFailuresPreprocessor ^fp);
        };
    }
}

#endif