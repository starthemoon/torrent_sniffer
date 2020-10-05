#include <string>
#include <vector>
#include <memory>
#include <bencoding.h>

using bencoding::BDictionary;
using bencoding::BList;
using bencoding::BString;
using bencoding::BInteger;
using bencoding::BItem;
using bencoding::encode;
using bencoding::decode;

// These three functions are implemented in dht
void handleRequest(std::shared_ptr<BDictionary>) {}
void handleResponse(std::shared_ptr<BDictionary>) {}
void handleError(std::shared_ptr<BDictionary>) {}


enum class krpcType {
    ping,
    find_node,
    get_peers,
    announce_peer,
};

std::vector<std::string> krpcTypeName = {
    "ping",
    "find_node",
    "get_peers",
    "announce_peer",
};

std::string makeQuery(const std::string& tid, enum krpcType type, std::shared_ptr<BItem> a) {
    std::shared_ptr<BDictionary> pBObject = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("q")},
        { BString::create("q"), BString::create(krpcTypeName[static_cast<int>(type)])},
        { BString::create("a"), a},
    });
    return encode(pBObject);
}

std::string makeResponse(const std::string& tid, std::shared_ptr<BItem> r) {
    std::shared_ptr<BDictionary> pBObject = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("r")},
        { BString::create("r"), r},
    });
    return encode(pBObject);
}

std::string makeError(const std::string& tid, std::shared_ptr<BItem> e) {
    std::shared_ptr<BDictionary> pBObject = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("e")},
        { BString::create("e"), e},
    });
    return encode(pBObject);
}

void handleMessage(std::string message) {
    try {
        // parse逻辑：
        // 将raw string解析为BOject
        // 根据BOject中"y"字段，判断
        // 这条message是请求q, 响应r
        // 还是错误e
        std::shared_ptr<BItem> pBObject = decode(message);
        auto pBDictionary = std::dynamic_pointer_cast<BDictionary>(pBObject);
        // 如果解析出的BOject不是BDictionary，那么不是我们要的结果
        if (pBDictionary && (*pBDictionary)[BString::create("y")] != nullptr) {
            auto pBString = std::dynamic_pointer_cast<BString>((*pBDictionary)[BString::create("y")]);
            // 如果解析出"y"所对应的值不是BString，那么不是我们要的结果
            if (pBString) {
                std::string type = pBString->value();
                if (type == "q")
                    handleRequest(pBDictionary);
                else if (type == "r")
                    handleResponse(pBDictionary);
                else if (type == "e")
                    handleError(pBDictionary);
            }
        }

    } catch (const bencoding::DecodingError &ex) {
        // the message is not a bencoding
        return;
    }
}