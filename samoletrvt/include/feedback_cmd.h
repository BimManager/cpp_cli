// Copyright 2019 Samolet LLC
// Author: kkozlov
// feedback_cmd.h

#ifndef FEEDBACK_CMD_H_
#define FEEDBACK_CMD_H_

namespace Samolet {
namespace Commands {
[ATT::TransactionAttribute(ATT::TransactionMode::Manual)]
[ATT::RegenerationAttribute(ATT::RegenerationOption::Manual)]
[ATT::JournalingAttribute(ATT::JournalingMode::UsingCommandData)]
public ref class FeedbackCmd : UI::IExternalCommand {
public:
  virtual UI::Result Execute(
      UI::ExternalCommandData ^cmdData,
      String ^%msg, DB::ElementSet ^elements);
};
}  // namespace Commands
}  // namespace Samolet

#endif  // FEEDBACK_CMD_H_
