//contribution by Tsadena
#include "MiniGit.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
namespace fs = std::filesystem;
MiniGit::MiniGit() {
    currentBranch = "main";
    refs.HEAD = "";
    refs.branches.clear();
}
void MiniGit::init() {
    if (!fs::exists(".minigit")) {
        fs::create_directory(".minigit");
        std::cout << "Initialized empty MiniGit repo.\n";
    } else {
        std::cout << "MiniGit already initialized.\n";
    }
}
void MiniGit::addFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cout << "File not found: " << filename << "\n";
        return;
    }
    std::string content((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
    Blob blob;
    blob.filename = filename;
    blob.content = content;
    blob.id = hashContent(content);
    stagedBlobs.push_back(blob);
    std::cout << "Staged: " << filename << "\n";
}
void MiniGit::commit(const std::string& message) {
    Commit newCommit;
    newCommit.message = message;
    newCommit.parent_id = refs.HEAD;
    newCommit.timestamp = std::to_string(std::time(nullptr));
    for (const auto& blob : stagedBlobs) {
        saveBlob(blob);
        newCommit.blob_ids.push_back(blob.id);
    }
    newCommit.id = "commit_" + newCommit.timestamp;
    refs.HEAD = newCommit.id;
    refs.branches[currentBranch] = newCommit.id;
    saveCommit(newCommit);
    saveReferences();
    stagedBlobs.clear();
    std::cout << "Committed: " << message << "\n";
}
void MiniGit::log() {
    std::string current = refs.HEAD;
    while (!current.empty()) {
        std::ifstream in(".minigit/" + current + ".commit");
        if (!in) break;
        std::string message, parent, timestamp;
        std::getline(in, message);
        std::getline(in, parent);
        std::getline(in, timestamp);
        std::cout << "Commit: " << current << "\n";
        std::cout << "Timestamp: " << timestamp << "\n";
        std::cout << "Message: " << message << "\n\n";
        current = parent;
    }
}
void MiniGit::branch(const std::string& name) {
    refs.branches[name] = refs.HEAD;
      std::ofstream out(".minigit/" + name + ".branch");
      out << refs.HEAD;
      std::cout << "Created branch: " << name << "\n";
  }
  void MiniGit::checkout(const std::string& name) {
      std::string targetCommit;
      // Try branch first
      if (refs.branches.count(name)) {
          targetCommit = refs.branches[name];
          currentBranch = name;
      } else {
          // Otherwise assume it's a commit hash
          targetCommit = name;
      }
      std::ifstream in(".minigit/" + targetCommit + ".commit");
      if (!in) {
          std::cout << "Invalid branch or commit: " << name << "\n";
          return;
      }
      std::string message, parent, timestamp;
      std::getline(in, message);
      std::getline(in, parent);
      std::getline(in, timestamp);
      std::vector<std::string> blob_ids;
      std::string id;
      while (std::getline(in, id)) {
          blob_ids.push_back(id);
      }
      // Overwrite working directory
      for (const auto& blob_id : blob_ids) {
          std::ifstream bIn(".minigit/" + blob_id + ".blob");
          std::string content((std::istreambuf_iterator<char>(bIn)), std::istreambuf_iterator<char>());
          std::ofstream out(blob_id + ".txt"); // or map back to original name if stored
          out << content;
      }
      refs.HEAD = targetCommit;
      std::cout << "Checked out: " << name << "\n";
  }
  std::string MiniGit::hashContent(const std::string& content) {
      std::hash<std::string> hasher;
      return std::to_string(hasher(content));
  }
  void MiniGit::saveBlob(const Blob& blob) {
      std::string path = ".minigit/" + blob.id + ".blob";
      std::ofstream out(path);
      out << blob.content;
  }
  void MiniGit::saveCommit(const Commit& commit) {
      std::string path = ".minigit/" + commit.id + ".commit";
      std::ofstream out(path);
      out << commit.message << "\n";
      out << commit.parent_id << "\n";
      out << commit.timestamp << "\n";
      for (const auto& id : commit.blob_ids)
              out << id << "\n";
      }
      void MiniGit::saveReferences() {
          std::ofstream out(".minigit/HEAD");
          out << refs.HEAD << "\n";
          for (const auto& [branch, id] : refs.branches) {
              std::ofstream bOut(".minigit/" + branch + ".branch");
              bOut << id;
          }
      }

