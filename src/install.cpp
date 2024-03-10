#include <iostream>
#include <string>
#include <vector>

#include "install.h"
#include "resolve.h"

void install(const std::vector<std::string> &packages) {
    for (const std::string &package : packages) {
        int status = resolve(package);
        if (status != 0) {
            std::cerr << "Failed to resolve " << package << std::endl;
        }
    }
}