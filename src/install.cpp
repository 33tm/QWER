#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

#include "qwer.h"
#include "simdjson.h"

void install(std::vector<std::string> packages) {
    for (auto package : packages) {
        int status = resolve(package);
        if (status != 0) {
            std::cerr << "Failed to resolve " << package << std::endl;
        }
    }
}