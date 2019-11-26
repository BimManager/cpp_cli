// Copyright 2019 Samolet LLC
// Author: kkozlov
// main.cpp

#using <System.dll>
#using <System.Diagnostics.Debug.dll>

using System::String;
using System::Diagnostics::Debug;

void
CopyFiles(String ^src, String ^dst);

int main(array<String^> ^args) {
    if (args->Length == 2)
        CopyFiles(args[0], args[1]);
    else {
#ifdef DEBUG
      Debug::WriteLine(String::Format("Fewer than two argument"));
#endif
    }
    return (0);
}

void CopyFiles(String ^src, String ^dst) {
    System::Threading::Thread::Sleep(5000);
#ifdef DEBUG
    Debug::WriteLine(String::Format("Copying ... "));
    Debug::WriteLine(String::Format("src: {0}; dst: {1}",
                                    src, dst));
#endif
    try {
        System::IO::File::Copy(src, dst, true);
#ifdef DEBUG
        Debug::WriteLine("The file has been copied successfully.");
#endif
    } catch (System::Exception ^ex) {
#ifdef DEBUG
        Debug::WriteLine(String::Format("{0}", ex->Message));
#endif
    }
}
