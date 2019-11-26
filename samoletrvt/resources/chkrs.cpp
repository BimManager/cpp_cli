/*
 *  chkrs.cpp
 */

#using <System.Drawing.dll>

typedef System::Resources::ResourceManager  ResourceManager;
typedef System::Reflection::Assembly        Assembly;
typedef System::String                      String;
typedef System::Object                      Object;

namespace IO = System::IO;

int main(array<String ^> ^args)
{
    if (args->Length != 3)
    {
        System::Console::WriteLine(
            "usage: chkrs rsfile assembly rsname");
        return (1);
    }
    ResourceManager ^rm;
    Assembly        ^asmb;
    Object          ^res;
    array<String^>  ^rsNames;
    IO::Stream	    ^sm;

    asmb = Assembly::Load(args[1]);
    rm = gcnew ResourceManager(args[0], asmb);
    rsNames = asmb->GetManifestResourceNames();
    int i = rsNames->Length;
    while (i--)
      System::Console::WriteLine(rsNames[i]);
    sm = asmb->GetManifestResourceStream(rsNames[0]);
    System::Console::WriteLine(sm->ToString());
    sm->Close();
    res = rm->GetObject(args[2]);
    if (res != nullptr)
        System::Console::WriteLine(
            String::Format("Resource {0} exists "
                           + "in the assembly: {1}",
                           args[2], asmb->FullName));
    else
        System::Console::WriteLine(
            String::Format("Resource {0} has not been found "
                           + "in the assembly: {1}.",
                           args[2], asmb->FullName));
    return (0);
}
