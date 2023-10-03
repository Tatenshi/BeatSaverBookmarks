#pragma once

#include <thread>
#include <string>
#include <filesystem>
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

#include "libcurl/shared/curl.h"
#include "libcurl/shared/easy.h"

#include "include/logger.hpp"

#define TIMEOUT 10

#define USER_AGENT std::string("BeatSaverBookmarks/" VERSION " (BeatSaber) (Oculus)").c_str()

#define SessionKey std::string("*INSERT YOUR SESSION ID HERE*")

namespace WebUtils {
    std::thread PostJSONAsync(std::string url, std::string data, std::function<void(long, std::string)> const& finished);
    std::thread PostJSONAsync(const std::string& url, std::string data, long timeout, std::function<void(long, std::string)> const& finished);
    std::thread GetJSONAsync(std::string url, std::function<void(long, bool, rapidjson::Document const&)> const& finished);
    std::thread GetAsync(std::string url, std::function<void(long, std::string)> const &finished, std::function<void(float)> const &progressUpdate);
    std::thread GetAsync(std::string url, long timeout, const std::function<void(long, std::string)>& finished, const std::function<void(float)>& progressUpdate);
    std::thread RequestAsync(std::string url, std::string method, long timeout, const std::function<void(long, std::string)>& finished, const std::function<void(float)>& progressUpdate);
    std::string getCookieFile();
    std::string query_encode(std::string s);
    std::size_t CurlWrite_CallbackFunc_StdString(void *contents, std::size_t size, std::size_t nmemb, std::string &s);

    struct ProgressUpdateWrapper {
        std::function<void(float)> progressUpdate;
        long length;
    };
}