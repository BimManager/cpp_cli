/*
 *	actionHandler.h
 */

#ifndef ACTION_HANDLER_H
# define ACTION_HANDLER_H

# using <System.dll>
# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace KKZ
{
  using namespace System;
  using namespace Autodesk::Revit::UI;
  using namespace Autodesk::Revit::DB;
 
  public ref class ActionHandler : IExternalEventHandler
  {
  public:
    virtual void	Execute(UIApplication ^uiapp);
    virtual String	^GetName(void);
  };
 
}

#endif
