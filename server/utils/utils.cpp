//
// Created by sazonov99 on 3/24/24.
//

#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include "utils.h"


std::string formatCharCount(const std::unordered_map<char, int> charCount, std::string gap) {
    std::stringstream result;
    result << '\n';
    for (const auto& pair : charCount) {
        result << pair.first;
        result <<":" << gap;
        result << std::to_string(pair.second);
        result << '\n';
    }
    return result.str();
}

std::string createCharCountTable(std::string msg) {

    std::stringstream result;
    std::string gap = "        ";
    std::unordered_map<char, int> charCount;
    for (char c:msg){
        if (isalpha(c)) {
            charCount[c]++;
        }
    }
    result <<"\n" << "message -" << " " << msg << formatCharCount(charCount, gap);
    std::cerr << result.str();
    return result.str();
}