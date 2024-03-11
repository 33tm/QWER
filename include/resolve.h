#ifndef RESOLVE_H
#define RESOLVE_H

struct Resolution {
    std::string name;
    std::string version;
    std::string url;
    std::string hash;
    std::vector<Package> dependencies;
};

std::vector<Resolution> resolve(const std::vector<Package> &packages);

#endif