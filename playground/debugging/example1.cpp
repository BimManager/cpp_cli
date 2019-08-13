/*
 *  example1.cpp
 *  build with: "cl /clr example1.cpp /FUSystem.dll"
 */

using System::String;
using System::Configuration::AppSettingsReader;
using System::Exception;

int main(array<String ^> ^args)
{
    try
    {
        AppSettingsReader   ^asr;
        String              ^date;

        asr = gcnew AppSettingsReader();
        date = asr->GetValue("DateFormat", String::typeid)->ToString();
    }
    catch(Exception ^e)
    {

    }
    return (0);
}