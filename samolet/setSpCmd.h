/*
 *  setSpCmd.h
 */

 #ifndef SET_SP_CMD_H
 # define SET_SP_CMD_H

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

            String                       ^GetName(void);
            array<String^>               ^GetValues(void);
            LinkedList<DB::Parameter^>   ^GetParameters(void);
            int                          GetCount(void);

            void            AddParameter(DB::Parameter ^p);
            static void     SetElemCount(size_t n);
        private:
            String                      ^_name;
            array<String^>              ^_values;
            LinkedList<DB::Parameter^>  ^_params;
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

        [TransactionAttribute(TransactionMode::Manual)]
        [RegenerationAttribute(RegenerationOption::Manual)]
        public ref class SetSpCmd : IExternalCommand
        {
        public:
            virtual Result  Execute(
                ExternalCommandData ^data,
                String ^%msg, ElementSet ^elements);

        private:
            Result                  CarryOutCmd(ExternalCommandData ^data,
                                        String ^%msg, ElementSet ^elements);
            String                  ^LocateParamsSource(void);
            IList<DB::Parameter^>   ^GetSps(UI::UIDocument ^uidoc);
            array<ValuesSet^>       ^FillValuesSets(String ^filePath);
            void                    ExtractParameters(UI::UIDocument ^uidoc,
                                        array<ValuesSet^> ^sets);
            void                    SetValues(DB::Document ^doc, CL::Hashtable ^selected,
                                        array<ValuesSet^> ^sets);
            //CL::Hashtable           ^ConvertDataToFillForm(array<ValuesSet^> ^sets);
        }; /* SetSpCmd */
    } /* Commands */

    namespace Utils
    {
        static int  GetValueFromConfigFile(String ^key, String ^%out);
    }
 }

 #endif