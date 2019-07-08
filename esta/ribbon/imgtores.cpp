/*
 *  imgtors
 */

#using <System.dll>
#using <System.Drawing.dll>
#using <PresentationCore.dll>
#using <WindowsBase.dll>

namespace IO = System::IO;

typedef System::String                      String;
typedef System::Uri                         Uri;
typedef System::Resources::ResourceWriter   ResourceWriter;
typedef System::Drawing::Image              Image;
typedef System::Drawing::Bitmap              Bitmap;
typedef System::Windows::Media::Imaging::BitmapImage    BitmapImage;

BitmapImage ^GetBitmapImg(String ^filepath)
{
    Uri ^uri;
    uri = gcnew Uri(filepath);
    return (gcnew BitmapImage(uri));
}

IO::MemoryStream    ^GetMemoryStream(String ^filepath)
{
    IO::MemoryStream    ^ms;
    Bitmap              ^bm;

    bm = gcnew Bitmap(Image::FromFile(filepath));
    ms = gcnew IO::MemoryStream();
    bm->Save(ms, System::Drawing::Imaging::ImageFormat::Png);
    return (ms);
}

int main(array<String ^> ^args)
{
    if (args->Length != 3)
    {
        System::Console::WriteLine(
            "usage: imgtors file.resources image imagename");
        return (1);
    }
    ResourceWriter  ^rw;

    rw = gcnew ResourceWriter(args[0]);
    //rw->AddResource(args[2], Bitmap::FromFile(args[1]));
    rw->AddResource(args[2], GetMemoryStream(args[1]));
    rw->Close();
    return (0);
}