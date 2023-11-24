//
// Created by goras on 10.11.2023.
//
#include <algorithm>

#include "string_utils.h"

std::string lowercase(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), std::tolower);
    return input;
}

std::vector<std::string> split(const std::string &txt, char ch) {
    std::vector<std::string> strs;

    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while (pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs;
}