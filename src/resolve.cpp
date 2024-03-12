#include <curl/curl.h>
#include <string>
#include <vector>

#include "install.h"
#include "resolve.h"
#include "simdjson.h"

static size_t write(char *ptr, size_t size, size_t nmemb, std::string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

std::vector<Resolution> resolve(const std::vector<Package> &packages) {
    CURL *handles[packages.size()];
    CURLM *curlm = curl_multi_init();
    std::vector<std::string> responses(packages.size());
    std::vector<Resolution> resolutions(packages.size());

    // for (Package &package : packages) {
    // TODO semver resolution
    // mut package.version
    // work on latest assumption
    // continue;
    // }

    for (size_t i = 0; i < packages.size(); i++) {
        handles[i] = curl_easy_init();
        std::string url = "https://registry.npmjs.org/" + packages[i].name + "/" + packages[i].version;
        curl_easy_setopt(handles[i], CURLOPT_URL, url.c_str());
        curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &responses[i]);
        curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, *write);
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

    simdjson::ondemand::parser parser;

    for (const std::string &response : responses) {
        simdjson::padded_string json(response);
        simdjson::ondemand::document package = parser.iterate(json);

        Resolution resolution;

        package["name"].get_string(resolution.name);
        package["version"].get_string(resolution.version);

        simdjson::ondemand::object dependencies;
        simdjson::error_code error = package["dependencies"].get(dependencies);

        if (!error) {
            for (auto dependency : dependencies) {
                Package package;
                package.name = std::string(dependency.unescaped_key().value());
                dependency.value().get_string(package.version);
                resolution.dependencies.push_back(package);
            }
        }

        package["dist"]["shasum"].get_string(resolution.hash);
        package["dist"]["tarball"].get_string(resolution.url);

        for (Package &package : resolution.dependencies) {
            std::cout << package.name << " " << package.version << std::endl;
        }

        resolutions.push_back(resolution);
    }

    return resolutions;
}