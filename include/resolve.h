#ifndef RESOLVE_H
#define RESOLVE_H

#include <unordered_map>

struct Resolution {
    std::string name;
    std::string version;
    std::unordered_map<std::string, std::string> dependencies;
};

std::vector<Resolution> resolve(const std::vector<Package> &packages);

#endif