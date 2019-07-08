/*
 *  rdrs.cpp
 */

#using <System.Drawing.dll>

typedef System::Resources::ResourceManager  ResourceManager;
typedef System::Drawing::Image              Image;
typedef System::Reflection::Assembly        Assembly;
typedef System::String                      String;

int main(array<String ^> ^args)
{
    if (args->Length != 3)
    {
        System::Console::WriteLine("usage: rdrs rsname assembly imgname");
        return (1);
    }
    ResourceManager ^rm;
    Assembly        ^asmb;
    Image           ^img;

    asmb = Assembly::Load(args[1]);
    rm = gcnew ResourceManager(args[0], asmb);
    img = (Image ^)rm->GetObject(args[2]);
    if (img != nullptr)
        System::Console::WriteLine("Well done!");
    return (0);
}