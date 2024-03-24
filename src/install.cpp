#include <filesystem>
#include <fstream>
#include <vector>

#include <curl/curl.h>
#include <zlib.h>

#include "install.h"
#include "resolve.h"

#ifdef _WIN32
#define OS "win32"
#elif defined(__linux__)
#define OS "linux"
#elif defined(__APPLE__)
#define OS "darwin"
#else
#define OS "unknown"
#endif

#ifdef __i386__
#define CPU "ia32"
#elif defined(__x86_64__)
#define CPU "x64"
#elif defined(__arm__)
#define CPU "arm"
#elif defined(__aarch64__)
#define CPU "arm64"
#else
#define CPU "unknown"
#endif

#ifdef __GLIBC__
#define LIBC "glibc"
#else
#define LIBC "musl"
#endif

static bool find(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

static size_t toFile(const char *ptr, size_t size, size_t nmemb, std::ofstream *stream) {
    stream->write(ptr, size * nmemb);
    return size * nmemb;
}

static unsigned int decimal(char octal[12]) {
    unsigned int value = 0;
    for (size_t i = 0; i < 10; i++) {
        value = value * 8 + octal[i] - '0';
    }
    return value;
}

struct Header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char modifed[12];
    char checksum[8];
    char type;
    char linkname[100];
};

void install(const std::vector<Package> &packages) {
    std::vector<Resolution> resolutions = resolve(packages);

    CURLM *curlm = curl_multi_init();
    std::vector<CURL *> handles(resolutions.size());
    std::ofstream files[resolutions.size()];

    std::filesystem::create_directory("node_modules");

    for (size_t i = 0; i < resolutions.size(); i++) {
        const auto &[name, version, os, cpu, libc, bin, dependencies, dist] = resolutions[i];

        if ((!os.empty() && !find(OS, os)) || (!cpu.empty() && !find(CPU, cpu)) || (!libc.empty() && !find(LIBC, libc))) {
            // account for ! prefix
            continue;
        }

        std::filesystem::path package = "node_modules" / std::filesystem::path(name);
        if (name[0] == '@') std::filesystem::create_directory(package.parent_path());

        CURL *handle = curl_easy_init();
        files[i].open(package.string() + ".tgz", std::ios::binary | std::ios::trunc);
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

    for (std::ofstream &file : files) {
        file.close();
    }

    for (auto &file : std::filesystem::recursive_directory_iterator("node_modules")) {
        if (file.is_directory() || file.path().extension() != ".tgz") continue;

        gzFile tgz = gzopen(file.path().c_str(), "rb");

        unsigned int size;
        unsigned char buffer[16384];
        std::vector<unsigned char> data;

        while (size = gzread(tgz, buffer, sizeof(buffer))) {
            data.insert(data.end(), buffer, buffer + size);
        }

        gzclose(tgz);

        std::vector<unsigned char>::iterator base = data.begin();

        while (true) {
            std::vector<unsigned char> head(base, base + 512);
            Header *header = reinterpret_cast<Header *>(head.data());
            base += 512;
            unsigned int size = decimal(header->size);
            if (base + size > data.end()) break;
            std::vector<unsigned char> payload(base, base + size);
            base += size + 512 - size % 512;

            std::filesystem::path path = "node_modules" / file.path().stem() / std::string(header->name).substr(8);
            std::filesystem::create_directories(path.parent_path());
            
            std::ofstream output(path);
            if (!output) return;

            output.write(reinterpret_cast<const char *>(payload.data()), size);
            output.close();
        }

        std::filesystem::remove(file.path());
    }
}