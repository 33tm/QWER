#ifndef RESOLVE_H
#define RESOLVE_H

struct Resolution {
    std::string name;
    std::string version;
    std::map<std::string, std::string> dependencies;
    struct {
        std::string integrity;
        std::string shasum;
        std::string tarball;
    } dist;
};

std::vector<Resolution> resolve(const std::vector<Package> &packages);

#endif