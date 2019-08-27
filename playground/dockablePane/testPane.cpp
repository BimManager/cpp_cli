/*
 *	testPane.cpp
 */

#include "testPane.h"

using namespace KKZ;

TestPane::TestPane(IExternalEventHandler ^handler)
{
  InitializeComponent();
  this->_extEvent = ExternalEvent::Create(handler);
}

void	TestPane::InitializeComponent(void)
{
  Grid		^grid;
  Button	^actionBtn;
  IAddChild	^holder;

  grid = gcnew Grid();
  actionBtn = gcnew Button();
  actionBtn->Content = "Click me";
  actionBtn->Click += gcnew RoutedEventHandler(this, &TestPane::OnActionClicked);
  holder = grid;
  holder->AddChild(actionBtn);
  holder = this;
  holder->AddChild(grid);
}

void	TestPane::SetupDockablePane(DockablePaneProviderData ^data)
{
  data->VisibleByDefault = true;
  data->InitialState->TabBehind =
    Autodesk::Revit::UI::DockablePanes::BuiltInDockablePanes::PropertiesPalette;
  data->InitialState->DockPosition = Autodesk::Revit::UI::DockPosition::Tabbed;
  data->FrameworkElement = this;
  this->Background = System::Windows::Media::Brushes::White;
  return ;
}

void	TestPane::OnActionClicked(Object ^s, RoutedEventArgs ^e)
{
  this->_extEvent->Raise();
}
