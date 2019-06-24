/*
 *  parse_spf.cpp
 */

#define EXIT_SUCCESS 0

namespace IO = System::IO;

typedef System::String String;

void    ParseFile(String ^filename);
void    ParseLine(String ^line);

int     main(array<String ^> ^argv)
{
    ParseFile(argv[0]);
    return (EXIT_SUCCESS);
}

void    ParseFile(String ^filename)
{
    IO::StreamReader    ^sr;
    String              ^line;

    sr = gcnew IO::StreamReader(filename, System::Text::Encoding::UTF8);
    if (sr == nullptr)
    {
        System::Console::WriteLine(L"Something has gone wrong.");
        return ;
    }

    System::Console::OutputEncoding = System::Text::Encoding::UTF8;
    while ((line = sr->ReadLine()) != nullptr)
    {
        if (line->StartsWith(L"PARA"))
            ParseLine(line);
    }
}

void    ParseLine(String ^line)
{
    array<String ^>     ^fields;
    size_t              i;

    fields = line->Split(gcnew array<wchar_t> {L'\t'});
    i = -1;
    while (++i < fields->Length)
        System::Console::WriteLine(fields[i]);
}
