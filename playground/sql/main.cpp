// Copyright Samolet LLC
// Author: kkozlov
// main.cpp

#using <System.dll>
#using <System.Data.dll>
#using <Npgsql.dll>
#using <System.Threading.dll>
#using <System.Configuration.dll>

System::String^	GetConnectionStringByName(System::String^ name) {
  System::Configuration::ConnectionStringSettings^	constr;
  System::String^					ret;

  constr = System::Configuration::ConfigurationManager::ConnectionStrings[name];
  if (constr != nullptr) {
    ret = constr->ConnectionString;
  }
  System::Console::WriteLine(ret);
  return (ret);
}

void	Read(System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^ task) {
  System::Data::Common::DbDataReader^	dr;

  System::Console::WriteLine("foo2");
  try {
    System::Console::WriteLine(task->IsCompleted.ToString());
    task->Result;
      System::Console::WriteLine("baz");
  } catch (System::Exception^ e) {
    System::Console::WriteLine(e->Message);
  }
}

void	Open(System::Threading::Tasks::Task^ task, System::Object^ connection) {
  System::Console::WriteLine(task->IsCompleted);
  
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlConnection^	con;
  System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^	reader_task;

  System::Console::WriteLine("Inside Open function");
  cmd = gcnew Npgsql::NpgsqlCommand();
  if ((con = dynamic_cast<Npgsql::NpgsqlConnection^>(connection)) != nullptr) {
      System::Console::WriteLine("Connection is not null!");
      System::Console::WriteLine(con->State);
  } else {
    System::Console::WriteLine("Connection is null!");
      return ;
  }
  cmd->Connection = con;
  cmd->CommandType = System::Data::CommandType::Text;
  cmd->CommandText = System::String::Format("SELECT * FROM Cars");
  reader_task = cmd->ExecuteReaderAsync();
  reader_task->ContinueWith(gcnew System::Action<
                            System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^>(&Read));
}

void	GettingStarted(void){
  System::String^		con_str;
  Npgsql::NpgsqlConnection^	connection;
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlDataReader^	reader;
  System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^	task;
  System::Data::Common::DbDataReader^	dr;
  System::Threading::Tasks::Task^	open_task;

  con_str =   GetConnectionStringByName("postgres");
  connection = gcnew Npgsql::NpgsqlConnection(con_str);
  try {
    open_task = connection->OpenAsync();
    System::Console::WriteLine(connection->State);
    open_task->ContinueWith(gcnew System::Action<System::Threading::Tasks::Task^,
			    System::Object^>(&Open), connection);
    cmd = gcnew Npgsql::NpgsqlCommand();
    cmd->Connection = connection;
    cmd->CommandType = System::Data::CommandType::Text;
    cmd->CommandText = System::String::Format("SELECT * FROM Cars");
    task = cmd->ExecuteReaderAsync();
    task->ContinueWith(gcnew System::Action<
    System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^>(&Read));
    System::Threading::Thread::Sleep(1000);
  } catch (System::Exception^ e) {
    System::Console::WriteLine(e->Message);
  } finally {
    connection->CloseAsync();
  }  
}

void	Sample1(void) {
  System::String^		con_str;
  Npgsql::NpgsqlConnection^	connection;
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlDataReader^	reader;

  con_str = GetConnectionStringByName("postgres");
  connection = gcnew Npgsql::NpgsqlConnection(con_str);
  try {
    connection->Open();
  } catch (System::Reflection::ReflectionTypeLoadException^ ex) {
    System::Console::WriteLine("Load Type:" + ex->Message);
  }/* catch (System::Exception^ ex) {
    System::Console::WriteLine("Ordinary exception: " + ex->Message);
    }*/
}

int	main(void) {
    GettingStarted();
  //  Sample1();
  return (0);
}
