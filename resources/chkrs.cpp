/*
 *  chkrs.cpp
 */

#using <System.Drawing.dll>

typedef System::Resources::ResourceManager  ResourceManager;
typedef System::Reflection::Assembly        Assembly;
typedef System::String                      String;
typedef System::Object                      Object;

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

    asmb = Assembly::Load(args[1]);
    rm = gcnew ResourceManager(args[0], asmb);
    res = rm->GetObject(args[2]);
    if (res != nullptr)
        System::Console::WriteLine(
            String::Format("Resource {0} exists.", args[2]));
    else
        System::Console::WriteLine(
            String::Format("Resource {0} has not been found.", args[2]));
    return (0);
}