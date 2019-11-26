// Copyright 2019 Samolet LLC
// Author: kkozlov
// main.cpp

#include <stdio.h>

#using <System.dll>

#define TEST_URL "http://10.30.154.247:8000/api/revit/raw-volumes/"

using namespace System;
using namespace System::Net;
using namespace System::IO;

String^
postjson(char *url, String ^json);

int	main(int argc, char **argv)
{
  //  printf("Fooo");
  Console::WriteLine("Response: {0}",
                     postjson(TEST_URL,
                              "{\"object\":\"edifice\"," +
                              "\"partition\":\"ARCH\"," +
                              "\"date\":\"9/26/2019\"}"));
  return (0);
}

String^
postjson(char *url, String ^json)
{
  HttpWebRequest	^req;
  HttpWebResponse	^res;
  StreamWriter		^sw;
  StreamReader		^sr;
  String		^ret;

  req = WebRequest::CreateHttp(gcnew String(url));
  req->ContentType = "application/json";
  req->ContentLength = json->Length;
  req->Method = "POST";
  req->DefaultWebProxy->Credentials =
      CredentialCache::DefaultNetworkCredentials;
  sw = gcnew StreamWriter(
      req->GetRequestStream());
  sw->Write(json);
  sw->Close();
  res = static_cast<HttpWebResponse^>(
      req->GetResponse());
  sr = gcnew StreamReader(
      res->GetResponseStream());
  ret = sr->ReadToEnd();
  sr->Close();
  return (ret);
}
