/*
 *  parse_spf.cpp
 */

#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_NO_ARGS 1

#define ERROR_MSG_1 "Error: no arguments."

#define PARAM           0
#define GUID            1
#define NAME            2
#define DATATYPE        3
#define DATACATEGORY    4
#define GROUP           5
#define VISIBLE         6
#define DESCRIPTION     7
#define USERMODIFIABLE  8
#define BINDING         9
#define CATEGORIES      10

namespace IO = System::IO;

typedef System::String String;

void                ParseFile(String ^filename);
array<String ^>    ^ParseLine(String ^line);

int     main(array<String ^> ^argv)
{
    if (!argv->Length)
    {
        printf(ERROR_MSG_1);

        return (EXIT_NO_ARGS);
    }
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

array<String ^>    ^ParseLine(String ^line)
{
    array<String ^>     ^fields;
    size_t              i;

    fields = line->Split('\t');
    if (fields->Length != 11)
        printf("The number of fields is not equal to 11.\n");
    i = 0;
    while (++i < fields->Length)
        System::Console::WriteLine(fields[i]);
    printf("++++++++++++++++++++++++++++++++++\n");
    return fields;
}


