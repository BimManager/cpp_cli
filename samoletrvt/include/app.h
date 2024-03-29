// Copyright 2019 Samolet LLC
// Author: kkozlov
// app.h

#ifndef APP_H_
# define APP_H_

# define TAB_NAME "Самолет"
# define BOQ_PANEL_NAME "Смета"
# define COMMON_PANEL_NAME "Общии"
# define INFO_PANEL_NAME "Инфо"

# define RS_NAME "icons"

namespace Samolet {
namespace Applications {
public ref class MainApp : Autodesk::Revit::UI::IExternalApplication {
 public:
  virtual Autodesk::Revit::UI::Result
  OnStartup(Autodesk::Revit::UI::UIControlledApplication ^uiapp);
  virtual Autodesk::Revit::UI::Result
  OnShutdown(Autodesk::Revit::UI::UIControlledApplication ^uiapp);
  
 private:
  void AddButtons(Autodesk::Revit::UI::RibbonPanel^ panel);
  void AddButtons(Autodesk::Revit::UI::RibbonPanel^ panel,
                  array<Autodesk::Revit::UI::PushButtonData^>^ btn_data);
  Autodesk::Revit::UI::PushButtonData^ CreatePushButton(
      System::String ^nameAndText, System::String ^iconName,
      System::String ^className);
  void AddExtraInfo(Autodesk::Revit::UI::PushButtonData ^btnData,
                    System::String ^avblClass, System::String ^toolTip);
}; 
}  // namespace Application
}  // namespace Samolet

#endif  // APP_H_
