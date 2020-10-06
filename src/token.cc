#include "token.h"
#include "utils.h"

TokenManager::TokenManager(int tokenLen=7, int timeoutMin=5)
    : tokenLen(tokenLen),
        timeoutMin(timeoutMin),
        mutex(),
        map() {}

std::string TokenManager::newToken(const std::string& ip) {
    std::lock_guard<std::mutex> lk(mutex);
    // 如果有对应的token并且没有超时
    if (map.find(ip) != map.end() &&
        isTimeout(map[ip].timePoint) == false) {
        return map[ip].token;
    } else {
        std::string tokenstr = randomString(tokenLen);
        map[ip] = Token(tokenstr);
        return tokenstr;
    }
}

bool TokenManager::check(const std::string& ip, const std::string& tokenstr) {
    std::lock_guard<std::mutex> lk(mutex);
    // 如果ip没有对应的token
    if (map.find(ip) == map.end())
        return false;
    // 如果对应的token超时了
    if (isTimeout(map[ip].timePoint))
        return false;
    // 既有对应的token并且没有超时
    return true;
}

bool TokenManager::isTimeout(std::chrono::system_clock::time_point tokenTimePoint) {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now-tokenTimePoint).count() >= timeoutMin;
}