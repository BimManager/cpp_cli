/*
 *  ifcexp.cpp
 */

#include "ifcexp.h"

using namespace Esta;
using namespace Esta::Commands;
using namespace Esta::Exporters;

int  Esta::GetValueFromConfigFile(String ^key, String ^%out)
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

Object  ^Esta::GetValueFromConfigFile(String ^key, int %err)
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

Autodesk::Revit::UI::Result IfcExportCmd::Execute(
    Autodesk::Revit::UI::ExternalCommandData ^cmdData,
    System::String ^%msg,
    Autodesk::Revit::DB::ElementSet ^elements)
{
#ifdef DEBUG
    Debug::WriteLine(String::Format("{0} has started.",
        Assembly::GetExecutingAssembly()->FullName));
#endif
    String  ^greeting;
    int     err;
    
    if ((err = GetValueFromConfigFile(GREETING_KEY, greeting)) == 1)
        greeting = String::Format("No key \"{0}\" exists.", GREETING_KEY);
    else if (err == 2)
        greeting = String::Format("No config file.");
    Autodesk::Revit::UI::TaskDialog::Show("Revit Template", greeting);
    return (Autodesk::Revit::UI::Result::Succeeded);
}

IfcExporter::IfcExporter(AS::Application ^app, String ^outDir)
    : _app(app), _outDir(outDir)
    {}

DB::Document    ^IfcExporter::OpenDocument(AS::Application ^app, String ^filePath)
{
    DB::ModelPath               ^modelPath;
    DB::OpenOptions             ^openOpts;
    DB::WorksetConfiguration    ^wsConfig;

    modelPath = DB::ModelPathUtils::
        ConvertUserVisiblePathToModelPath(filePath);
    openOpts = gcnew DB::OpenOptions();
    wsConfig = gcnew DB::WorksetConfiguration(
        DB::WorksetConfigurationOption::OpenAllWorksets);
    openOpts->SetOpenWorksetsConfiguration(wsConfig);
    return (app->OpenDocumentFile(modelPath, openOpts));
}

DB::View        ^IfcExporter::FindViewByName(DB::Document ^doc, String ^viewName)
{
    DB::FilteredElementCollector    ^col;
    DB::FilterRule                  ^nameRule;
    DB::ElementParameterFilter      ^viewNameFltr;
    DB::ElementClassFilter          ^viewClassFltr;
    DB::View                        ^view;

    col = gcnew DB::FilteredElementCollector(doc);
    nameRule = DB::ParameterFilterRuleFactory::CreateContainsRule(
            gcnew DB::ElementId(DB::BuiltInParameter::VIEW_NAME),
            viewName, false);
    viewNameFltr = gcnew DB::ElementParameterFilter(nameRule);
    viewClassFltr = gcnew DB::ElementClassFilter(DB::View::typeid);
    view = static_cast<DB::View ^>(col->WherePasses(viewClassFltr)
               ->WherePasses(viewNameFltr)
               ->WhereElementIsNotElementType()
               ->FirstElement());
    return (view);
}

void            IfcExporter::CreateDefaultThreeDView(UI::UIApplication ^uiapp)
{
    UI::RevitCommandId  ^create3DViewCmd;

    create3DViewCmd = UI::RevitCommandId::LookupPostableCommandId(
                        UI::PostableCommand::Default3DView);
    if (uiapp->CanPostCommand(create3DViewCmd))
        uiapp->PostCommand(create3DViewCmd);
}

DB::IFCExportOptions    ^IfcExporter::SetupIfcExportOptions(
                            String ^famMapFile, 
                            String ^psetsFile,
                            DB::ElementId ^viewId)
{
    DB::IFCExportOptions    ^expOpts;

    expOpts = gcnew DB::IFCExportOptions();
    expOpts->AddOption("IFCVersion", "IFC 2x3 Coordination View 2.0");
    expOpts->AddOption("ExportIFCCommonPropertySet", "true");
    expOpts->AddOption("ExportInternalRevitPropertySets", "true");
    expOpts->AddOption("ExportBaseQuantities", "true");
    expOpts->AddOption("IncludeSiteElevation", "true");
    expOpts->AddOption("ExportLinkedFiles", "false");
    expOpts->AddOption("ExportPartsAsBuildingElement", "true");
    expOpts->AddOption("ExportSolidModelRep", "true");
    expOpts->FamilyMappingFile = famMapFile ? famMapFile : STD_MAP_FILE;
    if (psetsFile)
    {
        expOpts->AddOption("ExportUserDefinedPsets", "true");
        expOpts->AddOption("ExportUserDefinedPsetsFileName", psetsFile);
    }
    return (expOpts);
}

UI::Result  IfcExporter::Export(DB::IFCExportOptions ^expOpts)
{
    UI::Result      res;
    DB::Transaction ^tx;
    UI::UIApplication   ^uiapp;
    DB::Document    ^doc;

    res = UI::Result::Failed;
    uiapp = gcnew UI::UIApplication(this->_app);
    doc = uiapp->ActiveUIDocument->Document;
    tx = gcnew DB::Transaction(doc);
    tx->Start("Export the model to ifc");
    doc->Export(this->_outDir, doc->Title, expOpts);
    if (DB::TransactionStatus::RolledBack == tx->RollBack())
        res = UI::Result::Succeeded;
    return (res);
}