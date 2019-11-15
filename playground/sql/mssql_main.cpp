// Copyright 2019 Samolet LLC
// Author: kkozlov
// mssql_main.cpp

#using <System.dll>
#using <System.Data.dll>
#using <System.Configuration.dll>

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Configuration;

void	QueryDatabase(void);
void	InsertIntoDatabase(String^ cmd_str);
String^	GetConnectionStringByName(String^ name);

int	main(void) {
  //  QueryDatabase();
  InsertIntoDatabase("INSERT Products (ProductId, ProductName, Price, "
                     "ProductDescription) VALUES ("
                     "777, 'Monkey Wrench', 500.00, 'N/A')");
  return (0);
}

void	QueryDatabase(void) {
  SqlConnection^	connection;
  SqlCommand^		command;
  SqlDataReader^	reader;
  
  connection = gcnew SqlConnection(GetConnectionStringByName("mssql"));
  try {
    command = gcnew SqlCommand();
    command->Connection = connection;
    command->CommandType = CommandType::Text;
    command->CommandText = "SELECT * from Products";
    connection->Open();
    reader = command->ExecuteReader();
    while (reader->Read())
      System::Console::WriteLine("{0}; {1}; {2}; {3}",
                                 reader[0], reader[1],
                                 reader[2], reader[3]);
    reader->Close();
  } catch (Exception^ ex) {
    System::Console::WriteLine("Exception: {0}", ex->Message);
  } finally {
    connection->Close();
  }
}

void	InsertIntoDatabase(String^ cmd_str) {
  SqlConnection^	connection;
  SqlCommand^		command;
  int			affected;
  
  connection = gcnew SqlConnection(GetConnectionStringByName("mssql"));
  try {
    command = gcnew SqlCommand();
    command->Connection = connection;
    command->CommandType = CommandType::Text;
    command->CommandText = cmd_str;
    connection->Open();
    affected = command->ExecuteNonQuery();
    System::Console::WriteLine("{0} have been affected", affected);
  } catch (Exception^ ex) {
    System::Console::WriteLine("{0}", ex->Message);
  } finally {
    connection->Close();
  }
}

String^	GetConnectionStringByName(String^ name) {
  ConnectionStringSettings^	constr;
  String^			ret;

  constr = System::Configuration::ConfigurationManager::ConnectionStrings[name];
  if (constr != nullptr) {
    ret = constr->ConnectionString;
  }
  System::Console::WriteLine(ret);
  return (ret);
}
