cl /LD /clr bank.cpp bank_unittests.cpp /AI"c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\ReferenceAssemblies\v2.0" 
mstest /testcontainer:bank.dll 

vcvars32.bat 
