#include <curl/curl.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include "qwer.h"
#include "install.h"
#include "resolve.h"

bool find(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

static size_t toFile(const char *ptr, size_t size, size_t nmemb, std::ofstream *stream) {
    stream->write(ptr, size * nmemb);
    return size * nmemb;
}

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions = resolve(packages);

    CURLM *curlm = curl_multi_init();
    std::vector<CURL *> handles(resolutions.size());
    std::ofstream files[resolutions.size()];

    if (!std::filesystem::exists("node_modules")) {
        std::filesystem::create_directory("node_modules");
    }

    for (size_t i = 0; i < resolutions.size(); i++) {
        const auto &[name, version, os, cpu, libc, bin, dependencies, dist] = resolutions[i];

        if ((!os.empty() && !find(OS, os)) || (!cpu.empty() && !find(CPU, cpu)) || (!libc.empty() && !find(LIBC, libc))) {
            continue;
        }

        if (name[0] == '@') {
            std::filesystem::create_directory("node_modules/" + name.substr(0, name.find('/')));
        }

        CURL *handle = curl_easy_init();
        files[i].open("node_modules/" + name + ".tgz", std::ios::binary | std::ios::trunc);
        curl_easy_setopt(handle, CURLOPT_URL, dist.tarball.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, *toFile);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &files[i]);
        curl_multi_add_handle(curlm, handle);
        handles.push_back(handle);
    }

    int remaining;

    do {
        if (curl_multi_perform(curlm, &remaining)) {
            break;
        }
    } while (remaining);

    for (CURL *handle : handles) {
        curl_multi_remove_handle(curlm, handle);
        curl_easy_cleanup(handle);
    }

    curl_multi_cleanup(curlm);
}