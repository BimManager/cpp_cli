// Copyright 2019 Samolet LLC
// Author: kkozlov
// web.h

#ifndef WEB_H_
# define WEB_H_

//#define POST_URL	"http://10.30.154.247:8000/api/revit/raw-volumes/"
#define RVT_PARAMS_URL    "https://sp.samoletgroup.ru/api/revit/revit-parameters/"
#define TOKEN           "Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9"

namespace Samolet {
namespace Web {
public ref class SamoletResult {
public:
    property int	Id;
    property String^	Guid;
    property String^	Name;
    property String^	ApiUrl;
    property String^	RevitParameter;
};

public ref class SamoletResponse {
public:
    property int                            	Count;
    property String^                         	Next;
    property String^                        	Previous;
    property CLG::IList<SamoletResult^>^	Results;
};

public ref class ReqResManager {
public:
  static String^		GetRequest(String ^url, String ^token);
  static SamoletResponse^	ParseResponse(String ^json);
  static String^		JsonRequest(String^ url, String^ method, String ^json);
};
}  // namespace Web
}  // namespace Samolet
#endif
