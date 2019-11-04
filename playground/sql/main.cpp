// Copyright Samolet LLC
// Author: kkozlov
// main.cpp

#using <System.dll>
#using <System.Data.dll>
#using <Npgsql.dll>
#using <System.ValueTuple.dll>
#using <System.Text.Json.dll>
#using <System.Threading.dll>

#using <System.Configuration.dll>

System::String^	GetConnectionStringByName(System::String^ name) {
  System::Configuration::ConnectionStringSettings^	constr;
  System::String^					ret;

  constr = System::Configuration::ConfigurationManager::ConnectionStrings[name];
  if (constr != nullptr) {
    ret = constr->ConnectionString;
  }
  return (ret);
}

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

void	Open(System::Threading::Tasks::Task^ task, System::Object^ connection) {
  System::Console::WriteLine(task->IsCompleted);
  
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlConnection^	con;
  System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^	reader_task;
    
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
    System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^>(&Read2));
}

void	GettingStarted(void){
  System::String^		con_str;
  Npgsql::NpgsqlConnection^	connection;
  Npgsql::NpgsqlCommand^	cmd;
  Npgsql::NpgsqlDataReader^	reader;
  System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^	task;
  System::Data::Common::DbDataReader^	dr;
  System::Threading::Tasks::Task^	open_task;

  //    con_str = "Server=127.0.0.1;Port=5432;Database=postgres;User Id=kkozlov;Password=kir1992;Timeout=15";
    con_str =   GetConnectionStringByName("postgres");
  //con_str = "Username=kkozlov;Host=localhost;Port=5432;Database=testdb;";
  connection = gcnew Npgsql::NpgsqlConnection(con_str);
  try {
    open_task = connection->OpenAsync();
    open_task->ContinueWith(gcnew System::Action<System::Threading::Tasks::Task^,
			    System::Object^>(&Open), connection);
    /*    open_task->ContinueWith([=](System::Threading::Tasks::Task^ task) {

	  });*/
    System::Console::WriteLine(connection->State);
    /*    while (connection->State != System::Data::ConnectionState::Open) {
      System::Console::WriteLine("Connecting");
      return ;
      }*/
    /*    if (System::Data::ConnectionState::Open == connection->State) {
      System::Console::WriteLine(System::String::Format("Connection to the database {0}"
							" has been established.", connection->Database));
    } else {
      System::Console::WriteLine(System::String::Format("No connection"));
      return;
      }*/
    //    open_task->Wait();
    /*    cmd = gcnew Npgsql::NpgsqlCommand();
    cmd->Connection = connection;
    cmd->CommandType = System::Data::CommandType::Text;
    cmd->CommandText = System::String::Format("SELECT * FROM Cars");
    task = cmd->ExecuteReaderAsync();
    //    task->ContinueWith(gcnew System::Action<System::Threading::Tasks::Task^>(&Read));
    task->ContinueWith(gcnew System::Action<
    System::Threading::Tasks::Task<System::Data::Common::DbDataReader^>^>(&Read2));*/
  } catch (Npgsql::NpgsqlException^ e) {
    System::Console::WriteLine(e->Message);
  } finally {
  }
  
}

int	main(void) {
  GetConnectionStringByName("postgres");
  GettingStarted();
  return (0);
}
