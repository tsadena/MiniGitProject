#pragma once
#include <string>
#include <vector>
struct Commit {
    std::string id;
    std::string message;
    std::string parent_id;
    std::vector<std::string> blob_ids;
    std::string timestamp;
};

