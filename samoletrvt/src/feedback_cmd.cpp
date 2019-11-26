// Copyright 2019 Samolet LLC
// Author: kkozlov
// feedback_cmd.cpp

#include "feedback_cmd.h"

//#include <stdlib.h>
#include "windows.h"
#include "shellapi.h"

#pragma comment(lib, "shell32.lib")

namespace Samolet {
namespace Commands {
UI::Result FeedbackCmd::Execute(
    UI::ExternalCommandData ^cmdData,
    String ^%msg, DB::ElementSet ^elements) {
#ifdef DEBUG

#endif
  //  system("start mailto:k.kozlov@samoletgroup.ru");
  ShellExecute(NULL, "open", "mailto:k.kozlov@samoletgroup.ru", NULL, NULL, 0);
  return (UI::Result::Succeeded);
}
}   // namespace Commands
}   // namespace Samolet
