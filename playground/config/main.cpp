/*
 *  main.c
 */

#define KEY1 "Foo"
#define APP_SET_SECTION "appSettings"

#using <System.dll> /* System::Collections::Specialized */ 
/* System::Configuration::AppSettingsReader */
#using <System.Configuration.dll> 
#using <System.Reflection.dll>

using System::String;
using System::Console;
using System::Configuration::ConfigurationManager;
using System::Configuration::AppSettingsReader;
using System::Configuration::Configuration;
using System::Configuration::AppSettingsSection;
using System::Reflection::Assembly;

int  GetValueFromConfigFile(String ^key, String ^%out)
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


/* Method 1: ConfigurationManager::AppSettings->Get(key) */
/* Method 2: Utilizing the AppSettingsReader class */
/* Mehtod 3: AppSettingsSection */
static int main(void)
{
    String              ^val;
    AppSettingsReader   ^asr;
    Assembly            ^asmbl;
    Configuration       ^config;
    AppSettingsSection  ^assec;
    int                 ret;

    //val = ConfigurationManager::AppSettings->Get(KEY1);
    /*val = ConfigurationManager::AppSettings[KEY1];
    Console::WriteLine("val = {0}; type = {1}; type = {2}",
        val, String::typeid, val->GetType());
    asr = gcnew AppSettingsReader();
    Console::WriteLine("val = {0}", 
        (int)asr->GetValue(KEY1, System::Int32::typeid) + 1);
    Console::WriteLine("{0}", Assembly::GetExecutingAssembly()->FullName);
    val = Assembly::GetExecutingAssembly()->Location;
    config = ConfigurationManager::OpenExeConfiguration(val);
    assec = (AppSettingsSection ^)config->GetSection(APP_SET_SECTION);
    Console::WriteLine("{0} = {1}", KEY1, assec->Settings[KEY1]->Value);*/
    if (!(ret = GetValueFromConfigFile(KEY1, val)))
        Console::WriteLine("Val = {0}", val);
    else if (ret == 1)        
        Console::WriteLine("No key");
    else if (ret == 2)
        Console::WriteLine("No config file");
    return (0);
}