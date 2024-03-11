#ifndef INSTALL_H
#define INSTALL_H

struct Package {
    std::string name;
    std::string version;
};

void install(const std::vector<Package> &packages);

#endif