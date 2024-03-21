#include <curl/curl.h>
#include <fstream>
#include <vector>

#include "install.h"
#include "resolve.h"

static size_t write(const char *ptr, size_t size, size_t nmemb, std::ofstream *stream) {
    stream->write(ptr, size * nmemb);
    return size * nmemb;
}

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions = resolve(packages);
    std::ofstream files[resolutions.size()];
    CURL *handles[resolutions.size()];
    CURLM *curlm = curl_multi_init();

    for (size_t i = 0; i < resolutions.size(); i++) {
        handles[i] = curl_easy_init();
        files[i].open(resolutions[i].name + ".tgz");
        curl_easy_setopt(handles[i], CURLOPT_URL, resolutions[i].dist.tarball.c_str());
        curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, *write);
        curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &files[i]);
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
}