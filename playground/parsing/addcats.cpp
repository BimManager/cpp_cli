/*
 *  addcats.cpp
 */

#include <stdlib.h>

#using <mscorlib.dll>
#using <System.dll>

#define GEN_RND_FN  System::IO::Path::GetRandomFileName()

typedef System::String  String;
typedef System::IO::FileInfo    FileInfo;
typedef System::IO::StreamReader    StreamReader;
typedef System::IO::StreamWriter    StreamWriter;

String  ^modifyLine(String ^line, String ^catfile)
{
    StreamReader    ^sr;
    String          ^catline;
    array<String ^> ^cat_id;

    sr = gcnew StreamReader(catfile, System::Text::Encoding::Unicode);
    while ((catline = sr->ReadLine()) != nullptr)
    {
        cat_id = catline->Split('\t');
        if (line->Contains(cat_id[0]))
        {
            sr->Close();
            //return (String::Format("{0}\t{1}", line, cat_id[1]));
            return (String::Format("{0}{1}", line, cat_id[1]));
        }
    }
    sr->Close();
    return (line);
}

int processFile(String ^classfile, String ^catfile)
{
    FileInfo        ^fi;
    StreamReader    ^sr;
    StreamWriter    ^sw;
    String          ^line;
    size_t          i;

    fi = gcnew FileInfo(classfile);
    if (!fi->Exists)
        exit(1);

    sr = fi->OpenText();
    sw = gcnew StreamWriter(GEN_RND_FN, true, System::Text::Encoding::Unicode);
    i = 0;
    while ((line = sr->ReadLine()))
    {
        line = modifyLine(line, catfile);
        sw->WriteLine(line);
    }
    sw->Close();
    sr->Close();
    return (0);
}


int main(array<String ^> ^args)
{
    if (args->Length != 2)
    {
        System::Console::WriteLine("usage: addcats classfile catfile");
        return (1);
    }
    return (processFile(args[0], args[1]));
}