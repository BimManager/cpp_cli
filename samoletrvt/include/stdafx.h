// Copyright 2019 Samolet LLC
// Author: kkozlov
// stdafx.h

#ifndef STDAFX_H_
# define STDAFX_H_

# define SUCCESS		0
# define UNSPECIFIED_ERROR	-1

# define TRACE(msg) System::Diagnostics::EventLog::WriteEntry("Application", msg)

# ifdef DEBUG
#  define LOG(msg) \
  System::Diagnostics::Debug::WriteLine(\
      System::String::Concat(System::String::Format("[RVT] "), msg));
//      System::String::Concat(System::String::Format("[{0}] ", __FUNCTION__), msg));
# else
#  define LOG(msg) (void)0
# endif

typedef System::Collections::Generic::LinkedList<Autodesk::Revit::DB::Element^>		LinkedListElement;
typedef System::Collections::Generic::LinkedList<Autodesk::Revit::DB::ElementId^>	LinkedListElementId;
typedef System::Collections::Generic::LinkedList<Autodesk::Revit::DB::Parameter^>	LinkedListParameter;
typedef System::Collections::Generic::LinkedList<Autodesk::Revit::DB::XYZ^>		LinkedListXYZ;
typedef System::Collections::Generic::LinkedList<Autodesk::Revit::DB::Category^>	LinkedListCategory;
typedef System::Collections::Generic::LinkedList<System::String^>			LinkedListString;


typedef System::Collections::IEnumerator 						IEnumerator;
typedef System::Collections::Generic::IEnumerator<System::String^>			IEnumeratorString;
typedef System::Collections::Generic::IEnumerator<Autodesk::Revit::DB::Parameter^>	IEnumeratorParameter;
typedef System::Collections::Generic::IEnumerator<Autodesk::Revit::DB::Element^>	IEnumeratorElement;
typedef System::Collections::Generic::IEnumerator<Autodesk::Revit::DB::ElementId^>	IEnumeratorElementId;
typedef System::Collections::Generic::IEnumerator<Autodesk::Revit::DB::GeometryObject^>	IEnumeratorGeometryObject;

typedef System::Collections::Generic::IEnumerable<System::Type^>			IEnumerableType;

typedef System::Collections::Generic::ICollection<System::String^>			ICollectionString;
typedef System::Collections::Generic::ICollection<Autodesk::Revit::DB::Element^>	ICollectionElement;
typedef System::Collections::Generic::ICollection<Autodesk::Revit::DB::ElementId^>	ICollectionElementId;
typedef System::Collections::Generic::ICollection<Autodesk::Revit::DB::XYZ^>		ICollectionXYZ;
typedef System::Collections::Generic::ICollection<Autodesk::Revit::DB::Category^>	ICollectionCategory;

typedef System::Collections::Generic::IDictionary<System::String^, System::String^>	IDictionaryStringString;
typedef System::Collections::Generic::IDictionary<System::String^, System::Object^>	IDictionaryStringObject;
typedef System::Collections::Generic::Dictionary<System::String^, System::String^>	DictionaryStringString;
typedef System::Collections::Generic::Dictionary<System::String^, System::Object^>	DictionaryStringObject;

typedef System::Collections::Generic::SortedDictionary<Autodesk::Revit::DB::XYZ^,
                                                     Autodesk::Revit::DB::ElementId^>	SortedDictionaryXYZElementId;

typedef System::Collections::Hashtable	Hashtable;

namespace Samolet {
using namespace System;
namespace REF = System::Reflection;
namespace RS = System::Resources;
namespace IO = System::IO;
namespace NET = System::Net;

namespace CF = System::Configuration;
namespace CL = System::Collections;
namespace CLG = System::Collections::Generic;

# ifdef DEBUG
using System::Diagnostics::Debug;
# endif

namespace Forms = System::Windows::Forms;
namespace WINFORMS = System::Windows::Forms;
namespace DWG = System::Drawing;
namespace Dwg = System::Drawing;
namespace IMG = System::Windows::Media::Imaging;

using System::Web::Script::Serialization::JavaScriptSerializer;
using System::Web::Script::Serialization::JavaScriptConverter;

namespace DB = Autodesk::Revit::DB;
namespace ES = Autodesk::Revit::DB::ExtensibleStorage;
namespace UI = Autodesk::Revit::UI;
namespace AS = Autodesk::Revit::ApplicationServices;
namespace ATT = Autodesk::Revit::Attributes;
namespace SEL = UI::Selection;
namespace EVT = UI::Events;

}  // namespace Samolet
#endif  // stdafx.h
