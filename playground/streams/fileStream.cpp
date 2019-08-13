/*
 *  fileStream.cpp
 */

#using <System.dll>

using System::String;
using System::Console;
using System::Uri;
using System::IO::FileStream;
using System::IO::FileInfo;
using System::IO::FileMode;

void    readfile(String ^filepath);

int main(array<String ^> ^args)
{
    interior_ptr<String ^>  pb;
    interior_ptr<String ^>  pe;
    Uri                     ^uri;
    if (args->Length != 0)
    {
        pb = &args[0];
        pe = pb + args->Length;
        while (pb < pe)
            Console::WriteLine("{0}", *pb++);
        readfile(args[0]);
    }
    uri = gcnew Uri("https://api.github.com");
    Console::WriteLine(uri->Host);
    return (0);
}

void    readfile(String ^filepath)
{
    FileInfo    ^fi;
    FileStream  ^fs;

    fi = gcnew FileInfo(filepath);
    if (!fi->Exists)
        return ;
    fs = gcnew FileStream(filepath, FileMode::Open);
    if (fs != nullptr)
    {
        Console::WriteLine("File length is {0} or {1}",
            fs->Length, fi->Length);
        fs->Close();
    }
}