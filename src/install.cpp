#include <iostream>
#include <vector>

#include "install.h"
#include "resolve.h"

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions;

    resolutions = resolve(packages);

    // for (const Resolution &resolution : resolutions) {
    //     std::cout << resolution.name << "@" << resolution.version << std::endl;
    // }
}