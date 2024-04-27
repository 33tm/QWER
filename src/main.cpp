#include <iostream>
#include <string>
#include <vector>

#include "install.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: qwer <command>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    std::vector<Package> packages;
    if (command == "install" || command == "i") {
        for (std::string package : std::vector(argv + 2, argv + argc)) {
            packages.push_back({ package, "latest" });
        }
        install(packages);
    }
    else {
        std::cerr << "Unknown command: " << argv[1] << std::endl;
        return 1;
    }

    install(packages);

    return 0;
}