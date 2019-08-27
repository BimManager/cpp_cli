/*
 *	testPane.h
 */

#ifndef TEST_PANE_H
# define TEST_PANE_H

# using <System.dll>
# using <System.Diagnostics.Debug.dll>

# using <WindowsBase.dll>
# using <PresentationCore.dll>
# using <PresentationFramework.dll>

# using <RevitAPI.dll>
# using <RevitAPIUI.dll>

namespace KKZ
{
  using System::EventHandler;
  using System::Diagnostics::Debug;
  
  using System::Windows::Controls::Page;
  using System::Windows::Controls::Grid;
  using System::Windows::Controls::Button;
  using System::Windows::RoutedEvent;
  using System::Windows::RoutedEventArgs;
  using System::Windows::RoutedEventHandler;
  using System::Windows::Markup::IAddChild;

  using namespace Autodesk::Revit::UI;
  using Autodesk::Revit::UI::IDockablePaneProvider;
  using Autodesk::Revit::UI::DockablePaneProviderData;
  using Autodesk::Revit::UI::DockablePaneState;
  
  public ref class TestPane : IDockablePaneProvider, Page
  {
  public:
    TestPane(IExternalEventHandler ^handler);
    virtual void
      SetupDockablePane(DockablePaneProviderData ^data);
  private:
    ExternalEvent	^_extEvent;
    
    void	InitializeComponent(void);
    void	OnActionClicked(Object ^s, RoutedEventArgs ^e);
  };
}

#endif
