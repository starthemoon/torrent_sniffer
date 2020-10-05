#include "krpc.h"
#include "utils.h"

std::vector<std::string> KRPC::krpcTypeName = {
    "q",
    "r",
    "e"
};

void KRPC::sendMessage(std::shared_ptr<BDictionary> pBObject) {
    if (messageSender) {
        messageSender(encode(pBObject));
    }
}

void KRPC::handleMessage(const std::string& message) {
    try {
        // parse逻辑：
        // 将raw string解析为BOject
        // 根据BOject中"y"字段，判断
        // 这条message是请求q, 响应r
        // 还是错误e
        std::shared_ptr<BItem> pBObject = decode(message);
        // 如果解析出的BOject不是BDictionary，那么不是我们要的结果
        auto pBDictionary = std::dynamic_pointer_cast<BDictionary>(pBObject);
        if (!pBDictionary) return;
        auto t = parseBDictionary<BString>(pBDictionary, "t");
        if (!t) return;
        auto y = parseBDictionary<BString>(pBDictionary, "y");
        if (!y) return;
        std::string ystr = y->value();
        if (ystr == "q" && queryHandler)
            queryHandler(pBDictionary);
        else if (ystr == "r" && responseHandler)
            responseHandler(pBDictionary);
        else if (ystr == "e" && errorHandler)
            errorHandler(pBDictionary);
    } catch (const bencoding::DecodingError &ex) {
        // the message is not a bencoding
        return;
    }
}