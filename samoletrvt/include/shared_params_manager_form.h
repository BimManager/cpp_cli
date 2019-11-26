// Copyright 2019 Samolet LLC
// Author: kkozlov
// shared_params_manager_form.h

#ifndef SHARED_PARAMS_MANAGER_FORM_H_
# define SHARED_PARAMS_MANAGER_FORM_H_

# define ACTION_IMPORT_FROM_FILE    0
# define ACTION_IMPORT_FROM_MODEL   1
# define ACTION_EXPORT_TO_FILE      2
# define ACTION_EXPORT_TO_MODEL     3

namespace Samolet {
  
namespace eventdata {
public ref class DismissedDialogEventArgs : public System::EventArgs {
public:
  DismissedDialogEventArgs(System::String ^filepath,
                           Autodesk::Revit::DB::ModelPath ^modelPath, char action);
  Autodesk::Revit::DB::ModelPath^	Modelpath(void);
  System::String^			Filepath(void);
  char            			Action(void);
private:
  System::String^			filepath_;
  Autodesk::Revit::DB::ModelPath^	modelpath_;
  char					action_;
};
}  // namespace eventdata

namespace gui {
public delegate void VoidDelegateVoid(void);

public ref class FilePicker : public System::Windows::Forms::Form {
public:
  FilePicker(void);
  event System::EventHandler  ^DialogDismissed;
protected:
  virtual void OnDialogDismissed(eventdata::DismissedDialogEventArgs ^e);
private:
  System::Windows::Forms::TableLayoutPanel^	_tab;
  System::Windows::Forms::TextBox^		_txtbox;
  System::String^				filepath_;
  Autodesk::Revit::DB::ModelPath^		modelpath_;

  void    InitializeComponent(void);
  void    InitializeTableLayout(System::Windows::Forms::TableLayoutPanel ^%tab);
  void    InitializeButton(System::String ^title,int col, int row,
                           System::EventHandler ^handler);
  void    InitializeTextBox(void);
  void    InitializeButtons(void);
  void    BringUpSaveDialog(void);
  void    BringUpOpenDialog(void);
  void    ExportImport(char action1, char action2, VoidDelegateVoid ^pfn);
  void    HandleExportImport(char action);

  void    OnSelectModelClicked(System::Object ^s, System::EventArgs ^e);
  void    OnImportClicked(System::Object ^s, System::EventArgs ^e);
  void    OnExportClicked(System::Object ^s, System::EventArgs ^e);
  void    OnCancelClicked(System::Object ^s, System::EventArgs ^e);
};
}  // namespace gui
}  // namespace Samolet

#endif
