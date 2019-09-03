/*
 *  selValuesCmd.h
 */

 #ifndef SEL_VALUES_CMD_H
 # define SEL_VALUES_CMD_H

 # include "stdafx.h"

 namespace Samolet
 {
    namespace Commands
    {
        public ref class ValuesSet : CL::IComparer, IComparable
        {
        public:
            ValuesSet(String ^name, array<String^> ^values);

            virtual int     Compare(Object ^o1, Object ^o2);
            virtual int     CompareTo(Object ^other);

            String                              ^GetName(void);
            array<String^>                      ^GetValues(void);
            CLG::LinkedList<DB::Parameter^>     ^GetParameters(void);
            size_t                              GetCount(void);

            void            AddParameter(DB::Parameter ^p);
            static void     SetElemCount(size_t n);
        private:
            String                      ^_name;
            array<String^>              ^_values;
            CLG::LinkedList<DB::Parameter^>  ^_params;
            size_t                      _paramsCount;
            static size_t               s_totalParams;
        }; /* ValuesSet */

        ref class ParamsSourceParser
        {
        public:
            static CL::Hashtable   ^ParseTxtFile(String ^filePath);
        }; /* ParamsSourceParser */

        public ref class AvailableUponSelection : UI::IExternalCommandAvailability
        {
        public:
            virtual bool IsCommandAvailable(UI::UIApplication ^uiapp, DB::CategorySet ^selCats);
        };

        [ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
        [ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)]
        public ref class SetSpCmd : UI::IExternalCommand
        {
        public:
            virtual UI::Result  Execute(
                UI::ExternalCommandData ^data,
                String ^%msg, DB::ElementSet ^elements);

        private:
            UI::Result              CarryOutCmd(UI::ExternalCommandData ^data,
                                        String ^%msg, DB::ElementSet ^elements);
            String                  ^LocateParamsSource(void);
            array<ValuesSet^>       ^FillValuesSets(String ^filePath);
            void                    ExtractParameters(UI::UIDocument ^uidoc,
                                        array<ValuesSet^> ^sets);
            void                    SetValues(DB::Document ^doc, CL::Hashtable ^selected,
                                        array<ValuesSet^> ^sets);
        }; /* selValuesCmd */
    } /* Commands */
 }

 #endif