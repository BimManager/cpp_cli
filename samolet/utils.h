/*
 *  utils.h
 */

#ifndef UTILS_H
# define UTILS_H

# include "stdafx.h"

namespace Samolet
{
    namespace Utils
    {
        public ref class ResourceManager
        {
        public:
            static IO::MemoryStream     ^GetMemoryStream(REF::Assembly ^asmb,
                                        String ^rsName, String ^imgName);
            static IMG::BitmapImage     ^MemoryStreamToImage(IO::MemoryStream ^ms);
        }; /* ResourceManager */

        public ref class ConfigFileManager
        {
        public:
            static int  GetValueFromConfigFile(String ^key, String ^%out);
        };
    } /* Utils */    
}

#endif