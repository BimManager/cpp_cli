// Copyright 2019 Samolet LLC
// Author: kkozlov
// ParamsControllerForm.h

#ifndef PARAMS_CONTROLLER_FORM_H
# define PARAMS_CONTROLLER_FORM_H

# include "family_params_cmd.h"

#define FILE_PATH System::IO::Path::Combine(System::IO::Path::GetTempPath(), "family_params_form.dat")

namespace Samolet {
namespace Gui {
using Samolet::Commands::FamilyParamDefinition;
[Serializable]  
public ref class ParamsControllerForm : Forms::Form {
public:
  ParamsControllerForm(void);
  FamilyParamDefinition^	GetParamDefinition(void);
private:
  void	InitializeComponent(void);
  void	SetupLayout(void);
  void	SetupLabels(void);
  void	SetupComboBoxes(void);
  void	SetupButtons(void);
  void	OnDeleteClicked(Object ^s, EventArgs ^e);
  void	OnCancelClicked(Object ^s, EventArgs ^e);
  FamilyParamDefinition	^_paramDef;
};
} // namespace Gui
} // namespace Samolet

#endif
