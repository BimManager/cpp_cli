/*
 *  utils.cpp
 */

#include "utils.h"

using namespace Samolet;

IO::MemoryStream    ^Utils::ResourceManager::GetMemoryStream(
    REF::Assembly ^asmb, String ^rsName, String ^imgName)
{
    RS::ResourceManager ^rm;

    rm = gcnew RS::ResourceManager(rsName, asmb);
    return (dynamic_cast<IO::MemoryStream ^>(rm->GetObject(imgName)));
}

IMG::BitmapImage    ^Utils::ResourceManager::MemoryStreamToImage(
                IO::MemoryStream ^ms)
{
    IMG::BitmapImage         ^icon;

    icon = gcnew IMG::BitmapImage();
    ms->Seek(0, IO::SeekOrigin::Begin);
    icon->BeginInit();
    icon->StreamSource = ms;
    icon->EndInit();
    ms->Close();
    return (icon);
}

int  Utils::ConfigFileManager::GetValueFromConfigFile(String ^key, String ^%out)
{
    CF::Configuration   ^config;
    int                  ret;

    config = CF::ConfigurationManager::OpenExeConfiguration(
            REF::Assembly::GetExecutingAssembly()->Location);
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