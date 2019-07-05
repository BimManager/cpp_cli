/*
 *  imgtors
 */

#using <System.Drawing.dll>

typedef System::String                      String;
typedef System::Resources::ResourceWriter   ResourceWriter;
typedef System::Drawing::Image              Image;

int main(array<String ^> ^args)
{
    if (args->Length != 3)
    {
        System::Console::WriteLine("usage: imgtors file.resources image imagename");
        return (1);
    }
    ResourceWriter  ^rw;

    rw = gcnew ResourceWriter(args[0]);
    rw->AddResource(args[2], Image::FromFile(args[1]));
    rw->Close();
    return (0);
}