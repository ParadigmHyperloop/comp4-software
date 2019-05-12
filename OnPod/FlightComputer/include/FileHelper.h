#include <string>
#ifndef FLIGHTCOMPUTER_FILEHELPER_H
#define FLIGHTCOMPUTER_FILEHELPER_H


class FileHelper {
private:
    static int _calls;

    public:
        FileHelper();
        static std::string GetCurrentDirectory();
};


#endif //FLIGHTCOMPUTER_FILEHELPER_H
