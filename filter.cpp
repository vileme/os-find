//
// Created by gankedbymom on 15.04.19.
//

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "filter.h"


void filter::add(const char *flagName, const char *flagValue) {
    if (strcmp(flagName, "-inum") == 0) {
        needNodeNumber = true;
        char *end;
        iNodeNumber = static_cast<ino_t>(std::strtoll(flagValue, &end, 10));
        if (*end) {
            std::cerr << "Bad number of inum" << "\n";
            exit(EXIT_FAILURE);
        }
    }
    if (strcmp(flagName, "-name") == 0) {
        needName = true;
        name = flagValue;
    }
    if (strcmp(flagName, "-size") == 0) {
        needSize = true;
        std::string s = flagValue;
        char *end;
        size = static_cast<off_t>(std::strtoll(flagValue + 1, &end, 10));
        if (*end) {
            std::cerr << "Bad size" << "\n";
            exit(EXIT_FAILURE);
        }
        if (s[0] == '+') {
            neededSize = GREATER;
        }
        else if (s[0] == '=') {
            neededSize = EQUAL;
        }
        else if (s[0] == '-') {
            neededSize = LESS;
        }
        else {
            std::cerr<<"Write controversial sign in a right format";
            exit(EXIT_FAILURE);
        }

    }
    if (strcmp(flagName, "-nlinks") == 0) {
        needNumberOfHardLinks = true;
        char *end;
        numberOfHardlinks = static_cast<nlink_t>(std::strtoll(flagValue, &end, 10));
        if (*end) {
            std::cerr << "Bad amount of nlinks" << "\n";
            exit(EXIT_FAILURE);
        }
    }
    if (strcmp(flagName, "-exec") == 0) {
        needExec = true;
        exec = flagValue;
    }
}
