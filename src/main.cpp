#include <iostream>
#include <string>
#include <vector>

#include "qwer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: qwer <command>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    if (command == "install" || command == "i") {
        std::vector<std::string> packages(argv + 2, argv + argc);
        install(packages);
    } else {
        std::cerr << "Unknown command: " << argv[1] << std::endl;
        return 1;
    }

    return 0;
}