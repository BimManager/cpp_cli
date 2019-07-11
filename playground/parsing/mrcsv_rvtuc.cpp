/*
 *  convert_to_rvt_uniclass.cpp
 */

#include <stdlib.h>
#include <stdio.h>

#using <System.dll>

typedef System::IO::StreamWriter    StreamWriter;
typedef System::IO::StreamReader    StreamReader;
typedef System::IO::FileInfo        FileInfo;
typedef System::String              String;

int     processFile(String ^infile, String ^outfile);
String  ^parseLine(String ^line);
size_t  numDots(String ^code);

int main(array<String ^> ^argv)
{
    StreamReader    ^sr;
    StreamWriter    ^sw;

    if (argv->Length !=2)
    {
        printf("usage: a.exe infile outfile\n");
        return (EXIT_FAILURE);
    }
    processFile(argv[0], argv[1]);
    return (EXIT_SUCCESS);
}

int processFile(String ^infile, String ^outfile)
{
    FileInfo        ^fi;
    StreamReader    ^sr;
    StreamWriter    ^sw;
    String          ^line;
    size_t          i;

    fi = gcnew FileInfo(infile);
    if (!fi->Exists)
        exit(1);
    sr = fi->OpenText();
    sw = gcnew StreamWriter(outfile, true, System::Text::Encoding::Unicode);
    i = 0;
    while ((line = sr->ReadLine()))
        if ((line = parseLine(line)) != nullptr)
            sw->WriteLine(line);
    sw->Close();
    sr->Close();
    return (0);
}

String  ^parseLine(String ^line)
{
    array<String ^> ^fields;

    fields = line->Split(',');
    if (fields->Length < 2 || fields[0]->Length == 0)
        return (nullptr);
    return (String::Format("{0}\t{1}\t{2}\t", fields[0], fields[1], numDots(fields[0]) + 1));
}

size_t  numDots(String ^code)
{
    size_t  out;
    int     index;

    out = 0;
    index = 0;
    while ((index = code->IndexOf('.', index)) != -1)
    {
        ++out;
        ++index;
    }
    return (out);        
}




