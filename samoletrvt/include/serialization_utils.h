// Copyright 2019 Samolet LLC
// Author: kkozlov
// serialization_utils.h

#ifndef SERIALIZATION_UTILS_H_
# define SERIALIZATION_UTILS_H_

//System::Environment::GetFolderPath(System::Environment::SpecialFolder::MyDocuments)

# define SUCCESS	0

namespace samoletrvt {
namespace utils {

public ref class SerializationUtils {
public:
  static int	SerializeInstanceIntoBinaryFormat(
      System::Object ^obj, System::IO::Stream ^stream);
  static System::Object	^DeserializeFromBinaryFormat(
      System::IO::Stream ^stream);
};
}  // namespace utils
}  // namespace samoletrvt

#endif
