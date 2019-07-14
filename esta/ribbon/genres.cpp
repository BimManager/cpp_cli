/*
 *  genres.cpp
 *  Convert an image into a resource
 */

#using <System.dll>
#using <System.Drawing.dll>
#using <PresentationCore.dll>
#using <WindowsBase.dll>

typedef System::String                      String;
typedef System::IO::MemoryStream            MemoryStream;
typedef System::Resources::ResourceWriter   ResourceWriter;
typedef System::Drawing::Bitmap             Bitmap;
typedef System::Drawing::Image              Image;

MemoryStream    ^GetMemoryStream(String ^filepath)
{
    MemoryStream    ^ms;
    Bitmap          ^bm;

    bm = gcnew Bitmap(Image::FromFile(filepath));
    ms = gcnew MemoryStream();
    bm->Save(ms, System::Drawing::Imaging::ImageFormat::Png);
    return (ms);
}

String          ^GetResourceName(String ^imgname)
{
    int     dot;

    dot = imgname->LastIndexOf('.');
    if (dot == -1)
        return (imgname);
    return (imgname->Substring(0, dot));
}

int             AddResources(String ^filename, array<String ^> ^imgnames)
{
    ResourceWriter  ^rw;
    size_t          i;

    rw = gcnew ResourceWriter(filename);
    i = imgnames->Length;
    while (--i)
        rw->AddResource(GetResourceName(imgnames[i]),
                GetMemoryStream(imgnames[i]));
    rw->Generate();
    rw->Close();
    return (0);
}

int             main(array<String ^> ^args)
{
    if (args->Length < 2)
    {
        System::Console::WriteLine(
            "usage: genres file.resources res1 [res2 ...]");
        return (1);
    }
    return (AddResources(args[0], args));
}

/*int main(array<String ^> ^args)
{
    ResourceWriter  ^rw;

    if (args->Length != 3)
    {
        System::Console::WriteLine(
            "usage: imgrs file.resources image imagename");
        return (1);
    }
    rw = gcnew ResourceWriter(args[0]);
    rw->AddResource(args[2], GetMemoryStream(args[1]));
    rw->Close();
    return (0);
}*/