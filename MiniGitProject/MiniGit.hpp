//Contribution by Yeabfikir
//contribution by Rahel Firdawork
#pragma once
#include <string>
#include <vector>
#include "blob.hpp"
#include "commit.hpp"
#include "references.hpp"
class MiniGit {
public:
    MiniGit();
    void init();
    void addFile(const std::string& filename);
    void commit(const std::string& message);
    void log();
    void branch(const std::string& name);
    void checkout(const std::string& name);
private:
    std::vector<Blob> stagedBlobs;
    References refs;
    std::string currentBranch = "main";
    std::string hashContent(const std::string& content);
    void saveBlob(const Blob& blob);
    void saveCommit(const Commit& commit);
    void loadReferences();
    void saveReferences();
};

