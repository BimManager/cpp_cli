/*
 *	main.cpp
 */

#using <System.dll>
#using <System.Data.dll>
#using <System.Configuration.dll>

namespace Sql = System::Data::SqlClient;
namespace Diag = System::Diagnostics;

using System::Configuration::ConfigurationManager;
//using System::Debug;

#define MEM_ALLOC_ERROR	1

int	readData(void)
{
  Sql::SqlConnection	^connection;
  Sql::SqlCommand     ^cmd;
  Sql::SqlDataReader     ^reader;

  Diag::Debug::Listeners->Add(gcnew
			      Diag::TextWriterTraceListener(System::Console::Out));
			      //     Diag::TextWriterTraceListener(System::Console::OpenStandardOutput()));
  try
    {
      Diag::Debug::WriteLine("===Debugging===");
      connection = gcnew Sql::SqlConnection();
      if (connection == nullptr)
	return (MEM_ALLOC_ERROR);
      #ifdef DEBUG
      Diag::Debug::WriteLine(
			     ConfigurationManager::ConnectionStrings["SqlConnection"]->ConnectionString);
      #endif
      connection->ConnectionString =
	ConfigurationManager::ConnectionStrings["SqlConnection"]->ConnectionString;
      connection->Open();
      #ifdef DEBUG
      Diag::Debug::WriteLine("Connection has been established.");
      #endif
      cmd = gcnew Sql::SqlCommand();
      cmd->Connection = connection;
      cmd->CommandType = System::Data::CommandType::Text;
      cmd->CommandText = 
        System::String::Format("SELECT TOP 5 name FROM spt_values");
      reader = cmd->ExecuteReader();
      while (reader->Read())
      {
        Diag::Debug::WriteLine(System::String::Format("name: {0}", reader["name"]));
      }
    }
  catch (Sql::SqlException ^e)
    {
      #ifdef DEBUG
      Diag::Debug::WriteLine("No connection. Something has gone tits-up.");
      #endif
      return (1);
    }
  finally
    {
      connection->Close();
      Diag::Debug::WriteLine("Connection has been closed successfully");
    }
  return (0);
}

int	main(void)
{
  return (readData());
}

