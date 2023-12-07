//
// Created by goras on 10.11.2023.
//

#ifndef DB_STRING_UTILS_H
#define DB_STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>


std::string lowercase(std::string input);

std::vector<std::string> split(const std::string &txt, char ch = ' ');

template<typename t>
t convertToNumber(const char *input) {
    t result;
    std::istringstream ss(input);
    if (!(ss >> result)) {
        throw std::exception((std::string("Invalid number: ") + std::string(input)).c_str());
    } else if (!ss.eof()) {
        throw std::exception((std::string("Trailing characters after number: ") + std::string(input)).c_str());
    }
    return result;
}

#endif //DB_STRING_UTILS_H
