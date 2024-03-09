#include <curl/curl.h>
#include <iostream>
#include <string>

#include "simdjson.h"

int main() {
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL" << std::endl;
        return 1;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, "https://tttm.us/ip");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        +[](void *ptr, size_t size, size_t nmemb, void *userdata) {
            ((std::string *)userdata)->append((char *)ptr, size * nmemb);
            return size * nmemb;
        });

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Failed cURL request" << std::endl;
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(response);

    std::cout << doc["ip"] << std::endl;

    auto json = simdjson::padded_string::load("package.json");
    simdjson::ondemand::document package = parser.iterate(json);

    std::cout << package["dependencies"] << std::endl;
    std::cout << package["devDependencies"] << std::endl;

    return 0;
}