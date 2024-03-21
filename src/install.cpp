#include <curl/curl.h>
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
    using std::filesystem::create_directory;

    std::vector<Resolution> resolutions = resolve(packages);
    std::ofstream files[resolutions.size()];
    std::vector<CURL *> handles(resolutions.size());
    CURLM *curlm = curl_multi_init();

    create_directory("node_modules");

    for (size_t i = 0; i < resolutions.size(); i++) {
        const auto &[name, version, os, cpu, dependencies, dist] = resolutions[i];

        if ((!os.empty() && !find(OS, os)) || (!cpu.empty() && !find(CPU, cpu))) {
            continue;
        }

        if (name[0] == '@') {
            create_directory("node_modules/" + name.substr(0, name.find('/')));
        }

        CURL *handle = curl_easy_init();
        files[i].open("node_modules/" + name + ".tgz");
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