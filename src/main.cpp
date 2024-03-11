#include <iostream>
#include <string>
#include <vector>

#include "install.h"

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     std::cerr << "Usage: qwer <command>" << std::endl;
    //     return 1;
    // }

    // std::string command = argv[1];
    // if (command == "install" || command == "i") {
    //     std::vector<std::string> packages(argv + 2, argv + argc);
    //     install(packages);
    // } else {
    //     std::cerr << "Unknown command: " << argv[1] << std::endl;
    //     return 1;
    // }

    std::vector<Package> packages;

    packages.push_back({"next", "latest"});
    packages.push_back({"schoologyapi", "latest"});

    install(packages);

    return 0;
}