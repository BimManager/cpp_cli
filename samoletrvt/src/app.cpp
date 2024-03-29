// Copyright 2019 Samolet LLC
// Author: kkozlov
// app.cpp

#include "app.h" 
#include "utils.h"
#include "events.h"

using namespace Samolet;
using namespace Samolet::Applications;

UI::Result MainApp::OnStartup(UI::UIControlledApplication ^uiapp) {
  UI::RibbonPanel     ^panel;
  UI::PushButtonData  ^btnData;

  uiapp->CreateRibbonTab(TAB_NAME);
  panel = uiapp->CreateRibbonPanel(TAB_NAME, BOQ_PANEL_NAME);
  AddButtons(panel, gcnew array<UI::PushButtonData^> {
      CreatePushButton("Выбрать\nЗначения", "select32x32",
                       "Samolet.Commands.SelValuesCmd"),
          CreatePushButton("Проверить", "valid32x32",
                           "Samolet.Commands.ValidationCmd"),
          CreatePushButton("JSON", "json32x32",
                           "Samolet.Commands.ParamsToJsonCmd"),
          CreatePushButton("Поиск\nUnique Id", "unique_id32x32",
                           "Samolet.Commands.SelectByUniqueId")
    });
  panel = uiapp->CreateRibbonPanel(TAB_NAME, COMMON_PANEL_NAME);
    AddButtons(panel, gcnew array<UI::PushButtonData^> {
          CreatePushButton("Параметры", "shared_params32x32",
                           "Samolet.Commands.SharedParamsManagerCmd"),
              CreatePushButton("Параметры\nСемейств", "remove32x32",
                               "Samolet.Commands.FamilyParameterRemoverCmd"),
              CreatePushButton("Простановка\nОтверстии", "openings32x32",
                               "Samolet.Commands.CutOpeningsCmd")
      });
    panel = uiapp->CreateRibbonPanel(TAB_NAME, INFO_PANEL_NAME);
    AddButtons(panel, gcnew array<UI::PushButtonData^> {
        CreatePushButton("Feedback", "feedback32x32",
                         "Samolet.Commands.FeedbackCmd")
      });
  uiapp->ApplicationClosing += gcnew 
      EventHandler<EVT::ApplicationClosingEventArgs^>
      (&Events::EventsManager::CopyNewVerAsmblOnClosing);
  uiapp->DialogBoxShowing += gcnew EventHandler<EVT::DialogBoxShowingEventArgs^>(
      &Events::EventsManager::OnDialogBoxShowing);
  return (UI::Result::Succeeded);
}

UI::Result MainApp::OnShutdown(UI::UIControlledApplication ^uiapp) {
    return (UI::Result::Succeeded);
}

void MainApp::AddButtons(UI::RibbonPanel^ panel) {
  UI::PushButtonData  ^btnData;
  
  btnData = CreatePushButton("Выбрать\nЗначение", "select32x32",
                               "Samolet.Commands.SelValuesCmd");
  /*  AddExtraInfo(btnData,
               "Samolet.Commands.AvailableUponSelection",
               "ToolTip");*/
  panel->AddItem(btnData);
  btnData = CreatePushButton("JSON",
                             "json32x32",
                             "Samolet.Commands.ParamsToJsonCmd");
  panel->AddItem(btnData);
  btnData = CreatePushButton("Проверить",
                               "valid32x32",
                               "Samolet.Commands.ValidationCmd");
  panel->AddItem(btnData);
  btnData = CreatePushButton("Параметры",
                               "shared_params32x32",
                               "Samolet.Commands.SharedParamsManagerCmd");
  panel->AddItem(btnData);
  btnData = CreatePushButton("Параметры Семейств",
                             "remove32x32",
                             "Samolet.Commands.FamilyParameterRemoverCmd");
  panel->AddItem(btnData);
  btnData = CreatePushButton("Поиск\nUnique Id",
                             "unique_id32x32",
                             "Samolet.Commands.SelectByUniqueId");
  panel->AddItem(btnData);
  panel->AddSeparator();
  btnData = CreatePushButton("Feedback",
                             "feedback32x32",
                             "Samolet.Commands.FeedbackCmd");
  panel->AddItem(btnData);
}

void MainApp::AddButtons(Autodesk::Revit::UI::RibbonPanel^ panel,
                         array<Autodesk::Revit::UI::PushButtonData^>^ btn_data) {
  int	i;

  i = -1;
  while (++i < btn_data->Length)
    panel->AddItem(btn_data[i]);  
}

UI::PushButtonData^
MainApp::CreatePushButton(String ^nameAndText,
                          String ^iconName, String ^className)
{
    UI::PushButtonData  ^btnData;
    REF::Assembly       ^asmb;
    IO::MemoryStream    ^ms;
    IMG::BitmapImage    ^icon;

    asmb = REF::Assembly::GetExecutingAssembly();
    btnData = gcnew UI::PushButtonData(
        nameAndText, nameAndText, asmb->Location, className);
    if ((ms = Utils::ResourceManager
         ::GetMemoryStream(asmb, RS_NAME, iconName)) != nullptr
        && (icon = Utils::ResourceManager
            ::MemoryStreamToImage(ms)) != nullptr)
      btnData->LargeImage = icon;
    return (btnData);
}

void    MainApp::AddExtraInfo(UI::PushButtonData ^btnData,
            String ^avblClass, String ^toolTip)
{
    btnData->AvailabilityClassName = avblClass;
    btnData->ToolTip = toolTip;
}
