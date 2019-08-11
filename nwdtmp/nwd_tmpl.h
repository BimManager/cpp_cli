/*
 *  nwd_tmpl.h
 */

#ifndef NWD_TMPL_H
# define NWD_TMPL_H

# using <System.Windows.Forms.dll>
# using <Autodesk.Navisworks.Api.dll>

namespace Api = Autodesk::Navisworks::Api;
namespace AppParts = Api::ApplicationParts;

typedef System::String String;

namespace Tmpl
{
    [Api::Plugins::PluginAttribute("Template", "ADSK", 
        ToolTip = "Template", DisplayName = "Template")]
    [Api::Plugins::AddInPluginAttribute(Api::Plugins::AddInLocation::AddIn)]
    ref class NwdTmpl : Api::Plugins::AddInPlugin
    {
    public:
        virtual int Execute( ... array<String ^> ^parameters) override;
    };
}

#endif


