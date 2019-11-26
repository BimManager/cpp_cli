// Copyright 2019 Samolet LLC
// Author: kkozlov
// web.cpp

#include "web.h"

namespace Samolet {
namespace Web {
String^ ReqResManager::GetRequest(String ^url, String ^token) {
    IO::Stream              ^sm;
    NET::HttpWebRequest     ^req;
    NET::HttpWebResponse    ^res;
    IO::StreamReader        ^sr;
    String                  ^data;

    req = NET::WebRequest::CreateHttp(url);
    req->Method = "GET";
    req->Headers->Add(NET::HttpRequestHeader::Authorization, token);
    req->ContentType = "application/x-www-form-urlencoded";
    res = static_cast<NET::HttpWebResponse^>(req->GetResponse());
    sm = res->GetResponseStream();
    sr = gcnew IO::StreamReader(sm);
    data = sr->ReadToEnd();
    res->Close();
    sr->Close();
    return (data);
}

SamoletResponse^ ReqResManager::ParseResponse(String ^json) {
    JavaScriptSerializer    ^parser;
    SamoletResponse         ^res;
    
    parser = gcnew JavaScriptSerializer();
    res = dynamic_cast<SamoletResponse^>(
        parser->Deserialize(json, SamoletResponse::typeid));
    return (res);
    }

String^ ReqResManager::JsonRequest(String^ url, String^ method, String ^json) {
  NET::HttpWebRequest^		req;
  NET::HttpWebResponse^		res;
  IO::StreamWriter^		sw;
  IO::StreamReader^		sr;
  String^			ret;
  IO::BinaryWriter^		br;
  array<unsigned char>^		bytes;

  req = NET::WebRequest::CreateHttp(url);
  req->ContentType = "application/json";
  req->Method = method;
  req->DefaultWebProxy->Credentials =
      NET::CredentialCache::DefaultNetworkCredentials;
  bytes = System::Text::Encoding::UTF8->GetBytes(json);
  req->ContentLength = bytes->Length;
  try {
    br = gcnew IO::BinaryWriter(req->GetRequestStream());
    br->Write(bytes);
  } catch (Exception^ e) {
#ifdef DEBUG
    LOG(e->Message);
#endif
  }
  finally {
    br->Close();
  }
  res = static_cast<NET::HttpWebResponse^>(
      req->GetResponse());
  sr = gcnew IO::StreamReader(
      res->GetResponseStream());
  ret = sr->ReadToEnd();
#ifdef DEBUG
  Debug::Write(String::Format("Response: {0}", ret));
#endif
  sr->Close();
  return (ret);
}
}  // namespace Web
}  // namespace Samolet
