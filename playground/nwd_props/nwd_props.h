/*
 *  nwd_tmpl.h
 */

#ifndef NWD_PROPS_H
# define NWD_PROPS_H

# using <System.Windows.Forms.dll>
# using <Autodesk.Navisworks.Api.dll>

namespace Api = Autodesk::Navisworks::Api;
namespace DocParts = Autodesk::Navisworks::Api::DocumentParts;
namespace AppParts = Api::ApplicationParts;

typedef System::String              String;
typedef System::Text::StringBuilder StrBuilder;

namespace Tmpl
{
    [Api::Plugins::PluginAttribute("GetProp", "ADSK", 
        ToolTip = "GetProp", DisplayName = "GetProp")]
    [Api::Plugins::AddInPluginAttribute(Api::Plugins::AddInLocation::AddIn)]
    ref class NwdTmpl : Api::Plugins::AddInPlugin
    {
    public:
        virtual int Execute( ... array<String ^> ^parameters) override;
    };
}

#endif


