#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "install.h"
#include "resolve.h"

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions;

    std::thread spinner([&resolutions, &packages] {
        size_t i = 0;
        const char *frames[] = {
            "⢀⠀",
            "⡀⠀",
            "⠄⠀",
            "⢂⠀",
            "⡂⠀",
            "⠅⠀",
            "⢃⠀",
            "⡃⠀",
            "⠍⠀",
            "⢋⠀",
            "⡋⠀",
            "⠍⠁",
            "⢋⠁",
            "⡋⠁",
            "⠍⠉",
            "⠋⠉",
            "⠋⠉",
            "⠉⠙",
            "⠉⠙",
            "⠉⠩",
            "⠈⢙",
            "⠈⡙",
            "⢈⠩",
            "⡀⢙",
            "⠄⡙",
            "⢂⠩",
            "⡂⢘",
            "⠅⡘",
            "⢃⠨",
            "⡃⢐",
            "⠍⡐",
            "⢋⠠",
            "⡋⢀",
            "⠍⡁",
            "⢋⠁",
            "⡋⠁",
            "⠍⠉",
            "⠋⠉",
            "⠋⠉",
            "⠉⠙",
            "⠉⠙",
            "⠉⠩",
            "⠈⢙",
            "⠈⡙",
            "⠈⠩",
            "⠀⢙",
            "⠀⡙",
            "⠀⠩",
            "⠀⢘",
            "⠀⡘",
            "⠀⠨",
            "⠀⢐",
            "⠀⡐",
            "⠀⠠",
            "⠀⢀",
            "⠀⡀",
            "  ",
            "  ",
            "  ",
            "  ",
            "  ",
            "  ",
            "  ",
            "  ",
        };
        // while (resolutions.empty()) {
        //     for (Package package : packages) {
        //         std::cout << package.name << std::right
        //                   << std::setw(50 - package.name.size())
        //                   << frames[i] << std::endl;
        //     }
        //     i = (i + 1) % 64;
        //     std::cout << "\033[" << packages.size() << "A";
        //     std::cout << package.name << std::right
        //     std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // }
        // for (Package package : packages) {
        //               << std::setw(50 - package.name.size())
        //               << "|" << std::endl;
        // }
    });

    resolutions = resolve(packages);

    for (const Resolution &resolution : resolutions) {
    }

    spinner.join();
}