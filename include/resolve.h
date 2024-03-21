#ifndef RESOLVE_H
#define RESOLVE_H

#include <map>

struct Resolution {
    std::string name;
    std::string version;
    std::vector<std::string> os;
    std::vector<std::string> cpu;
    std::vector<std::string> libc;
    std::map<std::string, std::string> bin;
    std::map<std::string, std::string> dependencies;
    struct {
        std::string tarball;
    } dist;
};

std::vector<Resolution> resolve(const std::vector<Package> &packages);

#endif