/*
 *  propsmgr.h
 */

#ifndef PROPS_MGR_H
# define PROPS_MGR_H

# using <System.Windows.Forms.dll>
# using <Autodesk.Navisworks.Api.dll>

namespace Forms = System::Windows::Forms;

namespace Api = Autodesk::Navisworks::Api;
namespace AppParts = Api::ApplicationParts;

typedef System::String String;
typedef System::Text::StringBuilder StringBuilder;

# define PLUGIN_NAME   "propsmgr"
# define DEV_ID        "KKOZ"
# define TOOLTIP       "Plugin Information"
# define DISPLAY_NAME   "Property Manager"   

namespace Esta
{
    [Api::Plugins::PluginAttribute(
        PLUGIN_NAME, DEV_ID,
        ToolTip= TOOLTIP, 
        DisplayName = DISPLAY_NAME)]
    [Api::Plugins::AddInPluginAttribute(
        Api::Plugins::AddInLocation::AddIn,
        Shortcut = "Ctrl+J")]
    ref class PropsMgr : Api::Plugins::AddInPlugin
    {
    public:
        virtual int Execute( ... array<String ^> ^parameters) override;

        static void PrintProperties(Api::ModelItem ^item);
    };
}

#endif


