// Copyright Samolet LLC
// Author: kkozlov
// main.cpp

#using <System.dll>
#using <System.Data.dll>
#using <Npgsql.dll>
#using <System.ValueTuple.dll>
#using <System.Text.Json.dll>
#using <System.Threading.dll>

void	Read2(System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^ task) {
  System::Data::Common::DbDataReader^	dr;

  System::Console::WriteLine("foo2");
  try {
    //    dr = task->Result;
    //    while(dr->Read())
    System::Console::WriteLine(task->IsCompleted.ToString());
    task->Result;
      System::Console::WriteLine("baz");
  } catch (System::Exception^ e) {
    System::Console::WriteLine(e->Message);
  }
}

void	Read(System::Threading::Tasks::Task^ task) {
  System::Console::WriteLine("foo");
}

void	GettingStarted(void){
  System::String^		con_str;
  Npgsql::NpgsqlConnection^	connection;
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlDataReader^	reader;
  System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^	task;
  System::Data::Common::DbDataReader^	dr;

  con_str = "Server=127.0.0.1;Port=5432;Database=testdb;UserId=kkozlov";
  //  con_str = "Username=kkozlov;Host=localhost;Port=5432;Database=testdb;";
  connection = gcnew Npgsql::NpgsqlConnection(con_str);
  try {
    connection->OpenAsync();
    System::Console::WriteLine(System::String::Format("Connection to the database {0}"
						      " has been established.", connection->Database));
    cmd = gcnew Npgsql::NpgsqlCommand();
    cmd->Connection = connection;
    cmd->CommandType = System::Data::CommandType::Text;
    cmd->CommandText = System::String::Format("SELECT * FROM Cars");
    task = cmd->ExecuteReaderAsync();
    //    task->ContinueWith(gcnew System::Action<System::Threading::Tasks::Task^>(&Read));
    task->ContinueWith(gcnew System::Action<
		       System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^>(&Read2));
  } catch (Npgsql::NpgsqlException^ e) {
    System::Console::WriteLine(e->Message);
  } finally {
  }
  
}

int	main(void) {
  GettingStarted();
  return (0);
}
