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

 #include <stdlib.h>
 #include <stdio.h>

 #define URL "https://www.google.com"

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
    property String ^ForPrice;
    property String ^EmPrice;       
};

public ref class Materials
{
public:
    property    int Count;
    String      ^Next;
    String      ^Previous;
    IList<Material^> ^Results;
};

void performWebRequest(String ^url);
Token ^getToken(String ^clientId, String ^clientSecret);
void GetMaterials(void);
void ParseJson(String ^json);

 int    main(void)
 {
    Person                      ^p1;
    Dictionary<String^,String^> ^jsonObj;
    char    *client_id;
    String  ^id;

    client_id = getenv("FORGE_CLIENT_ID");
    if (client_id != NULL)
        printf("client_id: %s\n", client_id);
    id = gcnew String(client_id);
    if (id != nullptr)
        System::Console::WriteLine(id);
    //free(client_id);
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
    GetMaterials();
    return (0);
 }

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

 void GetMaterials(void)
 {
    String          ^url;
    Stream          ^sm;
    HttpWebRequest  ^req;
    HttpWebResponse    ^res;
    StreamReader        ^sr;

    array<unsigned char> ^postBytes = System::Text::Encoding::ASCII->GetBytes("limit=1000");
    url = "https://sp.samoletgroup.ru/api/standards/kinds/";
    req = dynamic_cast<HttpWebRequest^>(WebRequest::Create(url));
    req->Method = "GET";
    req->Headers->Add("Authorization: Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9");
    //req->Headers->Add(System::Net::HttpRequestHeader::Authorization,
      //  "Token f00968f6c8ea9d5d7a3978651ef7aa9524f133a9");
    req->ContentType = "application/x-www-form-urlencoded";
    res = dynamic_cast<HttpWebResponse^>(req->GetResponse());
    sm = res->GetResponseStream();
    sr = gcnew StreamReader(sm);
    /*while ((url = sr->ReadLine()))
    {
        System::Console::WriteLine(url);
    }
    sr->Close();*/
    ParseJson(sr->ReadToEnd());
 }

 void ParseJson(String ^json)
 {
    JavaScriptSerializer    ^parser;
    Materials               ^mats;

    parser = gcnew JavaScriptSerializer();
    mats = dynamic_cast<Materials^>(
        parser->Deserialize(json, Materials::typeid));
    System::Console::WriteLine(String::Format("{0}; {1}; {2}; {3};\n {4}; {5}",
        mats->Count, mats->Next, mats->Previous, mats->Results->Count,
        mats->Results[0]->Name, mats->Results[0]->Guid));
 }

Token ^getToken(String ^clientId, String ^clientSecret)
{
    String          ^url;
    HttpWebRequest  ^req;

    url = "https://sp.samoletgroup.ru/api/standards/kinds";
    req = dynamic_cast<HttpWebRequest^>(WebRequest::Create(url));
    return (nullptr);
}