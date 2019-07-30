/*
 *  rvttmp.cpp
 */

#include "rvttmp.h"

namespace Firm
{
    Object  ^GetValueFromConfigFile(String ^key, int %err)
    {
        Configuration   ^config;

        config = ConfigurationManager::OpenExeConfiguration(
                Assembly::GetExecutingAssembly()->Location);
        if (config->HasFile)
        {  
            err = config->AppSettings->Settings[key] != nullptr ? 0 : 1;
            if (!err) 
                return ((String ^)config->AppSettings->Settings[key]->Value);
        }
        else
            err = 2;
        return (nullptr);
    }

    namespace Commands
    {
        Autodesk::Revit::UI::Result Command::Execute(
            Autodesk::Revit::UI::ExternalCommandData ^cmdData,
            System::String ^%msg,
            Autodesk::Revit::DB::ElementSet ^elements)
            {
                String  ^greeting;
                int     err;
                
                greeting = (String ^)GetValueFromConfigFile(GREETING_KEY, err);
                if (err == 1)
                    greeting = String::Format("No key \"{0}\" exists", GREETING_KEY);
                else if (err == 2)
                    greeting = String::Format("No config file");
                Autodesk::Revit::UI::TaskDialog::Show("Revit Template", greeting);
                return (Autodesk::Revit::UI::Result::Succeeded);
            }
    }
}