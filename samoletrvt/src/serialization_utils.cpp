// Copyright 2019 Samolet LLC
// Author: kkozlov
// serialization_utils.cpp

#include "serialization_utils.h"

namespace samoletrvt {
namespace utils {
namespace binary = System::Runtime::Serialization::Formatters::Binary;

int	SerializationUtils::SerializeInstanceIntoBinaryFormat(
    System::Object ^obj, System::IO::Stream ^stream) {
  binary::BinaryFormatter	^bf;

  bf = gcnew binary::BinaryFormatter();
  bf->Serialize(stream, obj);
  stream->Close();
#ifdef DEBUG
  System::Diagnostics::Debug::WriteLine("Serialization has completed successfully.");
#endif
  return (SUCCESS);
}

System::Object	^SerializationUtils::DeserializeFromBinaryFormat(
    System::IO::Stream ^stream) {
  binary::BinaryFormatter	^bf;
  System::Object		^obj;

  bf = gcnew binary::BinaryFormatter();
  obj = bf->Deserialize(stream);
  stream->Close();
  return (obj);
}
}  // namespace utils
}  // namespace samoletrvt
