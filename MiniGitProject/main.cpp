//contribution by Maryamawit Getu
#include <iostream>
#include <sstream>
#include <string>
#include "MiniGit.hpp"
int main() {
    MiniGit git;
    std::string line;
    while (true) {
        std::cout << "minigit> ";  // prompt
        if (!std::getline(std::cin, line)) break;  // quit on EOF
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd == "init") {
            git.init();
        } else if (cmd == "add") {
            std::string filename;
            iss >> filename;
            git.addFile(filename);
        } else if (cmd == "commit") {
            std::string flag, message;
            iss >> flag;
            std::getline(iss, message);
            if (flag == "-m" && !message.empty()) {
                message = message.substr(message.find_first_not_of(" "));
                git.commit(message);
            } else {
                std::cout << "Usage: commit -m <message>\n";
            }
        } else if (cmd == "log") {
            git.log();
        } else if (cmd == "branch") {
            std::string branchName;
            iss >> branchName;
            git.branch(branchName);
        } else if (cmd == "checkout") {
            std::string name;
            iss >> name;
            git.checkout(name);
        } else if (cmd == "exit" || cmd == "quit") {
            break;  // exit program
        } else {
            std::cout << "Unknown command: " << cmd << "\n";
        }
    }
    return 0;

}

