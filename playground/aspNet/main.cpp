/*
 *  main.cpp
 */

 #using <System.dll>
 #using <System.Runtime.Serialization.Json.dll>
 #using <System.Web.Extensions.dll>

 using namespace System;

 using System::Web::Script::Serialization::JavaScriptSerializer;
 using System::Collections::Generic::Dictionary;
 using System::Collections::Generic::IList;
 using System::Net::HttpWebRequest;
 using System::Net::HttpWebResponse;
 using System::Net::WebRequest;
 using System::IO::Stream;
 using System::IO::StreamReader;
 using System::Collections::Generic::IEnumerator;

 #include <stdlib.h>
 #include <stdio.h>

public ref class Person
{
public:
    property String ^Name;
    property bool   Active;
};

public ref class Token
{
public:
    property String ^TokenType;
    property String ^AccessToken;
    property int    ExpiresIn;
};

public ref class Material
{
public:
    property int    Id;
    property String ^Guid;
    property String ^Name;
    property String ^FotPrice;
    property String ^EmPrice;
};

public ref class Materials
{
public:
    property int        Count;
    property String     ^Next;
    property String     ^Previous;
    IList<Material^>    ^Results;
};

String ^GetRequest(String ^url, String ^token)
 {
    Stream          ^sm;
    HttpWebRequest  ^req;
    HttpWebResponse ^res;
    StreamReader    ^sr;
    String          ^json;

    req = dynamic_cast<HttpWebRequest^>(WebRequest::Create(url));
    req->Method = "GET";
    req->Headers->Add(System::Net::HttpRequestHeader::Authorization, token);
    req->ContentType = "application/x-www-form-urlencoded";
    res = dynamic_cast<HttpWebResponse^>(req->GetResponse());
    sm = res->GetResponseStream();
    sr = gcnew StreamReader(sm);
    json = sr->ReadToEnd();
    res->Close();
    sr->Close();
    sr->Close();
    return (json);
 }

 IList<Material^> ^ParseMaterials(String ^json)
 {
    JavaScriptSerializer    ^parser;
    Materials               ^mats;
    IEnumerator<Material^>  ^it;
    Material                ^m;

    parser = gcnew JavaScriptSerializer();
    mats = dynamic_cast<Materials^>(
        parser->Deserialize(json, Materials::typeid));
    return (mats->Results);
 }

void performWebRequest(String ^url);
Token ^ParseToken(String ^res);
Token ^getToken(String ^clientId, String ^clientSecret);

 int    main(void)
 {
    IEnumerator<Material^>   ^it;
    Material                ^m;

    it = ParseMaterials(
        GetRequest(
            "https://sp.samoletgroup.ru/api/standards/kinds/?limit=1000",
            "Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9"))->GetEnumerator();
    while (it->MoveNext())
    {
        m = it->Current;
        System::Console::WriteLine(
            String::Format("Name: {0}\nGuid: {1}\nFotPrice: {2}\nEmPrice: {3}",
            m->Name, m->Guid, m->FotPrice, m->EmPrice));
    }
 }

 /*
    //GetMaterials();
    Token   ^tk;
    char    *client_id;
    char    *client_secret;

    client_id = getenv("FORGE_CLIENT_ID");
    client_secret = getenv("FORGE_CLIENT_SECRET");
    if (client_id == NULL || client_secret == NULL)
        return (0);
    tk = getToken(gcnew String(client_id),
            gcnew String(client_secret));
    if (tk != nullptr)
        System::Console::WriteLine(tk->AccessToken);
    return (0);*/

 void performWebRequest(String ^url)
 {
    System::Net::WebRequest    ^req;
    System::Net::WebResponse   ^res;
    System::IO::StreamReader   ^sr;
    String                     ^line;

    req = System::Net::WebRequest::Create(url);
    req->Proxy = System::Net::WebRequest::GetSystemWebProxy();
    req->Proxy->Credentials = System::Net::CredentialCache::DefaultCredentials;
    res = req->GetResponse();
    System::Console::WriteLine(
        dynamic_cast<System::Net::HttpWebResponse^>(res)->StatusDescription);
    sr = gcnew System::IO::StreamReader(res->GetResponseStream());
    while ((line = sr->ReadLine()))
    {
        System::Console::WriteLine(line);
    }
    res->Close();
    sr->Close();
 }

 void parsingJson(void)
 {
    Person                      ^p1;
    Dictionary<String^,String^> ^jsonObj;

    String ^json = "{'Name':'Foo','Active': true}";
    JavaScriptSerializer ^js = gcnew JavaScriptSerializer();
    p1 = dynamic_cast<Person^>(js->Deserialize(json, Person::typeid));
    jsonObj = dynamic_cast<Dictionary<String^,String^>^>(
        js->Deserialize(json, Dictionary<String^,String^>::typeid));
    if (p1 != nullptr)
        System::Console::WriteLine(
            String::Format("Name: {0}; Active:{1}", p1->Name, p1->Active));
    if (jsonObj != nullptr)
        System::Console::WriteLine(
            String::Format("Name: {0}; Active:{1}", jsonObj["Name"], jsonObj["Active"]));
 }

Token ^ParseToken(String ^res)
{
    JavaScriptSerializer    ^parser;
    Token                   ^t;

    parser = gcnew JavaScriptSerializer();
    t = dynamic_cast<Token^>(
        parser->Deserialize(res, Token::typeid));
    return (t);
}

Token ^getToken(String ^client_id, String ^client_secret)
{
    String          ^url;
    Stream          ^rsm;
    HttpWebRequest  ^req;
    HttpWebResponse ^res;
    StreamReader    ^sr;

    System::Console::WriteLine("Inside getToken");
    array<unsigned char> ^postBytes = 
        System::Text::Encoding::ASCII->GetBytes(
            "client_id" + System::Uri::EscapeDataString(client_id) +
            "&client_secret" + System::Uri::EscapeDataString(client_secret) +
            "&grant_type" + System::Uri::EscapeDataString("client_credentials") +
            "&scope" + System::Uri::EscapeDataString("data:read"));
            /*String::Format("client_id={0}&client_secret={1}&"
            "grant_type=client_credentials&scope=data:read",
            client_id, client_secret));*/
    url = "https://developer.api.autodesk.com/authenticaion/v1/authenticate";
    req = dynamic_cast<HttpWebRequest^>(WebRequest::Create(url));
    req->Proxy = System::Net::WebRequest::GetSystemWebProxy();
    req->Proxy->Credentials = System::Net::CredentialCache::DefaultCredentials;
    req->Method = "POST";
    req->ContentType = "application/x-www-form-urlencoded";
    req->ContentLength = postBytes->Length;
    /*req->Headers->Add("Authorization: Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9");
    req->Headers->Add(System::Net::HttpRequestHeader::Authorization,
        "Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9");*/
    System::Console::WriteLine("About to get a request stream");
    rsm = req->GetRequestStream();
    rsm->Write(postBytes, 0, postBytes->Length);
    rsm->Close();
    System::Console::WriteLine("About to send a request");
    res = dynamic_cast<HttpWebResponse^>(req->GetResponse());
    System::Console::WriteLine(res->Headers[0]);
    sr = gcnew StreamReader(res->GetResponseStream());
    res->Close();
    return (ParseToken(sr->ReadToEnd()));
}