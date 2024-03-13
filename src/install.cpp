#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "install.h"
#include "resolve.h"

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions;

    std::thread spinner([&resolutions, &packages] {
        while (true) {
            for (Package package : packages) {
                std::cout << package.name << std::right
                          << std::setw(50 - package.name.size())
                          << "/" << std::endl;
            }
            if (!resolutions.empty()) {
                break;
            }
            std::cout << "\033[" << packages.size() << "A";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    resolutions = resolve(packages);

    spinner.join();
}