//
// Created by sazonov99 on 3/24/24.
//

#include <string>
#include <unordered_map>
#include <sstream>
#include "utils.h"


std::string formatCharCount(const std::unordered_map<char, int> charCount) {
    std::stringstream result;
    result << '\n';
    for (const auto& pair : charCount) {
        result << pair.first;
        result << ' ';
        result << std::to_string(pair.second);
        result << '\n';
    }
    return result.str();
}

std::string createCharCountTable(std::string msg) {
    std::string result;
    std::unordered_map<char, int> charCount;
    for (char c:msg){
        if (isalpha(c)) {
            charCount[c]++;
        }
    }
    return formatCharCount(charCount);
}