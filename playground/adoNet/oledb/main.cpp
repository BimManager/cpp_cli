/*
 *	main.cpp
 */

#using <System.dll>
#using <System.Data.dll>
#using <System.Configuration.dll>

namespace OleDb = System::Data::OleDb;
namespace Diag = System::Diagnostics;

using System::Configuration::ConfigurationManager;
//using System::Debug;

#define MEM_ALLOC_ERROR	1

int	readData(void)
{
  OleDb::OleDbConnection	^connection;
  OleDb::OleDbCommand     ^cmd;
  OleDb::OleDbDataReader     ^reader;

  Diag::Debug::Listeners->Add(gcnew
			      Diag::TextWriterTraceListener(System::Console::Out));
			      //     Diag::TextWriterTraceListener(System::Console::OpenStandardOutput()));
  try
    {
      Diag::Debug::WriteLine("===Debugging===");
      connection = gcnew OleDb::OleDbConnection();
      if (connection == nullptr)
	return (MEM_ALLOC_ERROR);
      #ifdef DEBUG
      Diag::Debug::WriteLine(
			     ConfigurationManager::ConnectionStrings["OleDbConnection"]->ConnectionString);
      #endif
      connection->ConnectionString =
	ConfigurationManager::ConnectionStrings["OleDbConnection"]->ConnectionString;
      connection->Open();
      #ifdef DEBUG
      Diag::Debug::WriteLine("Connection has been established.");
      #endif
      cmd = gcnew OleDb::OleDbCommand();
      cmd->Connection = connection;
      cmd->CommandType = System::Data::CommandType::Text;
      cmd->CommandText = 
        System::String::Format("SELECT * FROM [t1$]");
      reader = cmd->ExecuteReader();
      while (reader->Read())
      {
        Diag::Debug::WriteLine(System::String::Format("Col1: {0}", reader["Col1"]));
      }
    }
  catch (OleDb::OleDbException ^e)
    {
      #ifdef DEBUG
      Diag::Debug::WriteLine(e->Message);
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

