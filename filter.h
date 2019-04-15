//
// Created by gankedbymom on 15.04.19.
//

#ifndef FIND_FILTER_H
#define FIND_FILTER_H


#include <sys/types.h>
#include <string>

class filter {
public:
    enum neededSize {
        GREATER,EQUAL,LESS
    };
    ino_t iNodeNumber;
    nlink_t numberOfHardlinks;
    std::string name;
    off_t  size;
    neededSize neededSize;
    std::string exec;
    bool needNodeNumber = false;
    bool needNumberOfHardLinks = false;
    bool needName = false;
    bool needSize = false;
    bool needExec = false;
    void add(const char* flagName,const char* flagValue);
};


#endif //FIND_FILTER_H
