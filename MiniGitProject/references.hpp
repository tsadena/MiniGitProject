#pragma once
#include <string>
#include <unordered_map>
struct References {
    std::string HEAD;
    std::unordered_map<std::string, std::string> branches;
};



