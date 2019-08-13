/*
 *  main.cpp
 */

#define KEY1 "Foo"
#define APP_SET_SECTION "appSettings"

#using <System.dll> /* System::Collections::Specialized */ 
/* System::Configuration::AppSettingsReader */
#using <System.Configuration.dll> 
#using <System.Reflection.dll>
#using <System.Runtime.Serialization.dll>
#using <System.Runtime.Serialization.Formatters.dll>
#using <System.Runtime.Serialization.Formatters.Soap.dll>

using System::String;
using System::Console;
using System::Configuration::ConfigurationManager;
using System::Configuration::AppSettingsReader;
using System::Configuration::Configuration;
using System::Configuration::AppSettingsSection;
using System::Reflection::Assembly;
using System::Runtime::Serialization::Formatters::Soap::SoapFormatter;

[System::SerializableAttribute]
ref class Gaff
{
public:
    Gaff(String ^addr)
    {
        this->_address = addr;
        this->_id = this->GetHashCode().ToString();
    }
    String  ^GetAddress()
    {
        return (this->_address);
    }
private:
    String  ^_address;
    [System::NonSerializedAttribute]
    String  ^_id;
};

static void serialise()
{
    Gaff                ^pad1;
    SoapFormatter ^sf;

    pad1 = gcnew Gaff("1452 Parkway Street");
    sf = gcnew SoapFormatter();
    sf->Serialize(Console::OpenStandardOutput(), pad1);
}

/* Method 1: ConfigurationManager::AppSettings->Get(key) */
/* Method 2: Utilizing the AppSettingsReader class */
/* Mehtod 3: AppSettingsSection */
static void useconfig()
{
    String              ^val;
    AppSettingsReader   ^asr;
    Assembly            ^asmbl;
    Configuration       ^config;
    AppSettingsSection  ^assec;

    val = ConfigurationManager::AppSettings->Get(KEY1);
    Console::WriteLine("val = {0}; type = {1}; type = {2}",
        val, String::typeid, val->GetType());
    asr = gcnew AppSettingsReader();
    Console::WriteLine("val = {0}", 
        (int)asr->GetValue(KEY1, System::Int32::typeid) + 1);
    Console::WriteLine("{0}", Assembly::GetExecutingAssembly()->FullName);
    val = Assembly::GetExecutingAssembly()->Location;
    config = ConfigurationManager::OpenExeConfiguration(val);
    assec = (AppSettingsSection ^)config->GetSection(APP_SET_SECTION);
    Console::WriteLine("{0} = {1}", KEY1, assec->Settings[KEY1]->Value);
}

static int main(void)
{
    serialise();
    return (0);
}