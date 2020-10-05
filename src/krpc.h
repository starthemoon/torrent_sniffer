#pragma onece
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "bencoding.h"

using bencoding::BDictionary;
using bencoding::BList;
using bencoding::BString;
using bencoding::BInteger;
using bencoding::BItem;
using bencoding::encode;
using bencoding::decode;



// These three functions are implemented in dht
// extern void handleQuery(std::shared_ptr<BDictionary>);
// extern void handleResponse(std::shared_ptr<BDictionary>);
// extern void handleError(std::shared_ptr<BDictionary>);

class KRPC {
public:

    enum class krpcType {
        query,
        response,
        error,
    };

    static std::vector<std::string> krpcTypeName;

    static std::function<void(const std::string&)> messageSender;

    static std::function<void(std::shared_ptr<bencoding::BDictionary>)> queryHandler;
    static std::function<void(std::shared_ptr<bencoding::BDictionary>)> responseHandler;
    static std::function<void(std::shared_ptr<bencoding::BDictionary>)> errorHandler;

    // 将消息数据进行bencode编码，并执行发送
    static void sendMessage(std::shared_ptr<BDictionary> pBObject);

    // 获取消息，进行bencode解码并将消息上呈给DHT节点
    static void handleMessage(const std::string& message);
};

