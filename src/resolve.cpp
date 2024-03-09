#include <curl/curl.h>
#include <iostream>
#include <string>

#include "qwer.h"
#include "simdjson.h"

int resolve(std::string package) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
        return 1;
    }
    
    std::cout << "Installing " << package << "..." << std::endl;

    long status;
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, ("https://registry.npmjs.com/" + package).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        +[](char *ptr, size_t size, size_t nmemb, std::string *userdata) {
            userdata->append(ptr, size * nmemb);
            return size * nmemb;
        });

    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
    curl_easy_cleanup(curl);

    if (status != 200) {
        std::cerr << "Package " << package << " not found" << std::endl;
        return 1;
    }

    simdjson::ondemand::parser parser;
    simdjson::ondemand::document registry = parser.iterate(response);

    auto version = registry["dist-tags"]["latest"];
    auto target = registry["versions"][version];

    std::cout << "Installing " << package << "@" << version << std::endl;
    std::cout << target["dist"]["tarball"] << std::endl;

    return 0;
}