#include <iostream>
#include <vector>

#include "install.h"
#include "resolve.h"

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions = resolve(packages);
}