/*
 *  ifcexp.h
 */

#ifndef RVT_TMP_H
# define RVT_TMP_H

# using <mscorlib.dll>
# using <System.dll>
# using <System.Configuration.dll>
# using <System.Reflection.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>
//# using <RevitAPIIFC.dll>

# define GREETING_KEY "greeting"

# define LOG(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

# define STD_MAP_FILE "c:\\ProgramData\\Autodesk\\RVT 2019\\exportLayers-ifc-iai.txt"

namespace Esta
{
    using System::Object;
    using System::String;
    using System::Configuration::ConfigurationManager;
    using System::Configuration::Configuration;
    using System::Reflection::Assembly;
    using System::Diagnostics::Debug;

    namespace DB = Autodesk::Revit::DB;
    namespace UI = Autodesk::Revit::UI;
    namespace AS = Autodesk::Revit::ApplicationServices;
    //namespace IFC = Autodesk::Revit::IFC;

    int  GetValueFromConfigFile(String ^key, String ^%out);
    Object  ^GetValueFromConfigFile(String ^key, int %err);

    namespace Commands
    {
        [Autodesk::Revit::Attributes::TransactionAttribute(
            Autodesk::Revit::Attributes::TransactionMode::Manual)] /* ReadOnly */
        [Autodesk::Revit::Attributes::RegenerationAttribute(
            Autodesk::Revit::Attributes::RegenerationOption::Manual)] 
        [Autodesk::Revit::Attributes::JournalingAttribute(
            Autodesk::Revit::Attributes::JournalingMode::UsingCommandData)] /* NoCommandData */
        public ref class IfcExportCmd : Autodesk::Revit::UI::IExternalCommand
        {
        public:
            virtual Autodesk::Revit::UI::Result Execute(
                Autodesk::Revit::UI::ExternalCommandData ^cmdData,
                System::String ^%msg,
                Autodesk::Revit::DB::ElementSet ^elements);
        };
    }

    namespace Exporters
    {
        ref class IfcExporter
        {
        public:
            IfcExporter(AS::Application ^app, String ^outDir);
            DB::IFCExportOptions    ^SetupIfcExportOptions(
                                        String ^famMapFile, 
                                        String ^psetsFile,
                                        DB::ElementId ^viewId);
            UI::Result  Export(DB::IFCExportOptions ^expOpts);
        private:
            AS::Application ^_app;
            String          ^_outDir;
            bool            ^_toClose;

            DB::Document            ^OpenDocument(AS::Application ^app,
                                        String ^filePath);
            DB::View                ^FindViewByName(DB::Document ^doc,
                                        String ^viewName);
            void                    CreateDefaultThreeDView(UI::UIApplication ^uiapp);
        };
    }

    namespace Utils
    {
        public ref class Utils
        {
        };
    }
}

#endif