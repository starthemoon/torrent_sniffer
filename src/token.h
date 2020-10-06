#pragma onece

#include <string>
#include <chrono>
#include <mutex>
#include <unordered_map>


struct Token {
    std::string token;
    std::chrono::system_clock::time_point timePoint;
    Token()
        : token(),
            timePoint() {}

    Token(std::string token) 
        : token(token),
            timePoint(std::chrono::system_clock::now()) {}
};

// token管理器，用于生成一个token，
// 并将生成的token与node ip关联起来
class TokenManager {

public:

    // 设定超时时间
    TokenManager(int tokenLen, int timeoutMin);

    // 传入node ip地址，返回tokenstr
    std::string newToken(const std::string& ip);

    // 传入node ip地址以及tokenstr，检查是否合法（匹配且不超时）
    bool check(const std::string& ip, const std::string& tokenstr);

private:
    int tokenLen;
    int timeoutMin;
    std::mutex mutex;
    // ip -> token
    std::unordered_map<std::string, struct Token> map;

    // 检查是否超时
    bool isTimeout(std::chrono::system_clock::time_point);

};