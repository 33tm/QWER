#include <curl/curl.h>
#include <string>
#include <vector>

#include "glaze/glaze.hpp"
#include "install.h"
#include "resolve.h"

size_t write(char *ptr, size_t size, size_t nmemb, std::string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

// fix duplicate dependencies
std::vector<Resolution> resolve(const std::vector<Package> &packages) {
    CURL *handles[packages.size()];
    CURLM *curlm = curl_multi_init();
    std::vector<Resolution> resolutions;
    std::string responses[packages.size()];

    // for (Package &package : packages) {
    // TODO semver resolution
    // mut package.version
    // work on latest assumption
    // continue;
    // }

    for (size_t i = 0; i < packages.size(); i++) {
        Package package = packages[i];
        handles[i] = curl_easy_init();
        std::string url = "https://registry.npmjs.org/" + package.name + "/latest";
        curl_easy_setopt(handles[i], CURLOPT_URL, url.c_str());
        curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, *write);
        curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &responses[i]);
        curl_multi_add_handle(curlm, handles[i]);
    }

    int remaining;

    while (remaining) {
        if (curl_multi_perform(curlm, &remaining)) {
            break;
        }
    }

    for (CURL *handle : handles) {
        curl_multi_remove_handle(curlm, handle);
        curl_easy_cleanup(handle);
    }

    curl_multi_cleanup(curlm);

    std::vector<Package> dependencies;

    for (const std::string &response : responses) {
        Resolution resolution;
        glz::parse_error error = glz::read<glz::opts{.error_on_unknown_keys = false}>(resolution, response);
        for (const auto &[name, version] : resolution.dependencies) {
            std::cout << name << "@" << version << std::endl;
            dependencies.push_back({name, version});
        }
        resolutions.push_back(resolution);
    }

    if (!dependencies.empty()) {
        std::vector<Resolution> resolved = resolve(dependencies);
        resolutions.insert(resolutions.end(), resolved.begin(), resolved.end());
    }

    return resolutions;
}