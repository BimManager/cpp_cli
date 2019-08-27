/*
 *	actionHandler.cpp
 */

#include "actionHandler.h"

using namespace KKZ;

void	ActionHandler::Execute(UIApplication ^uiapp)
{
  Document	^doc;

  doc = uiapp->ActiveUIDocument->Document;
  TaskDialog::Show("Gen", String::Format("Title: {0}", doc->Title));
}

String	^ActionHandler::GetName(void)
{
  return ("Action Handler");
}


