#include <stdio.h>

//#ifdef WINDOWS
//#include <direct.h>
#define GetCurrentDir getcwd
#include <unistd.h>

#include "FileHelper.h"


FileHelper::FileHelper() {}


std::string FileHelper::GetCurrentDirectory()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    return std::string(buff);
}