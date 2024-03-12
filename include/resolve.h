#ifndef RESOLVE_H
#define RESOLVE_H

struct Resolution {
    std::string name;
    std::string version;
    std::vector<Package> dependencies;
    std::string hash;
    std::string url;
};

std::vector<Resolution> resolve(const std::vector<Package> &packages);

#endif