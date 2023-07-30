//
// Created by mrtsima on 4/21/23.
//

#ifndef TSIMULATOR_UTILS_H
#define TSIMULATOR_UTILS_H


#include <string>

class Utils {

    static bool isBranch(const std::__cxx11::basic_string<char>& s);

    static bool isBranchHelper(const std::string &prefix);

    static bool funCall(std::string s);

    static bool isJMP(std::string jmp);

    static bool isLegal(std::string s);
public:
    static int checker(const std::string &left, std::string right, const std::string &line);

    static bool pcRelative(std::string s);
};


#endif //TSIMULATOR_UTILS_H
