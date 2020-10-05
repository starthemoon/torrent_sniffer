#include "dht.h"
#include "utils.h"

std::vector<std::string> DHT::queryTypeName = {
    "ping",
    "find_node",
    "get_peers",
    "announce_peer",
};

std::unordered_map<std::string, enum DHT::queryType> DHTnameQueryType = {
    {"ping", DHT::queryType::ping},
    {"find_node", DHT::queryType::find_node},
    {"get_peers", DHT::queryType::get_peers},
    {"announce_peer", DHT::queryType::announce_peer},
};

DHT::DHT() 
    : transMan(new TransactionManager()),
        rTable(new RoutingTable()) {}

void DHT::sendQuery(enum queryType type, std::shared_ptr<BItem> a, const std::string id="") {
    std::string tid = id.empty() ? transMan->getNextID() : id;
    std::shared_ptr<BDictionary> pBDictionary = makeQuery(tid, type, a);
    transMan->newTrans(tid, pBDictionary);
    KRPC::sendMessage(pBDictionary);
}

void DHT::sendResonpse(const std::string& tid, std::shared_ptr<BItem> r) {
    std::shared_ptr<BDictionary> pBDictionary = makeResponse(tid, r);
    KRPC::sendMessage(pBDictionary);
}

void DHT::sendError(const std::string& tid, std::shared_ptr<BItem> e) {
    std::shared_ptr<BDictionary> pBDictionary = makeError(tid, e);
    KRPC::sendMessage(pBDictionary);
}

std::shared_ptr<BDictionary> DHT::makeQuery(const std::string& tid, enum queryType type, std::shared_ptr<BItem> a) {
    std::shared_ptr<BDictionary> pBDictionary = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("q")},
        { BString::create("q"), BString::create(queryTypeName[static_cast<int>(type)])},
        { BString::create("a"), a},
    });
    return pBDictionary;
}

std::shared_ptr<BDictionary> DHT::makeResponse(const std::string& tid, std::shared_ptr<BItem> r) {
    std::shared_ptr<BDictionary> pBDictionary = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("r")},
        { BString::create("r"), r},
    });
    return pBDictionary;
}

std::shared_ptr<BDictionary> DHT::makeError(const std::string& tid, std::shared_ptr<BItem> e) {
    std::shared_ptr<BDictionary> pBDictionary = BDictionary::create({
        { BString::create("t"), BString::create(tid) },
        { BString::create("y"), BString::create("e")},
        { BString::create("e"), e},
    });
    return pBDictionary;
}

void DHT::handleQuery(std::shared_ptr<BDictionary> pBDictionary) {
    auto t = parseBDictionary<BString>(pBDictionary, "t");
    if (!t) return;
    std::string tstr = t->value();

    auto q = parseBDictionary<BString>(pBDictionary, "q");
    if (!q) return;
    std::string qstr = q->value();
    
    auto a = parseBDictionary<BDictionary>(pBDictionary, "a");
    if (!a) return;
    
    auto id = parseBDictionary<BString>(a, "id");
    if (!id) return;
    std::string idstr = id->value();
    // 告诉鸡哥这个是个好节点
    // tell ge ji this good
    // todo
    if (qstr == "ping") {
        // 问鸡哥我的node id是多少
        std::shared_ptr<BItem> mineID;
        sendResonpse(tstr, mineID);
    } else if (qstr == "find_node") {
        auto target = parseBDictionary<BString>(a, "target");
        if (target) {
            std::string targetstr = target->value();
            // 问鸡哥与targetstr最接近的是什么,得到一个字符串组，
            // 组成一个BOjbect
            std::shared_ptr<BItem>  findRet;
            sendResonpse(tstr, findRet);
        }
    } else if (qstr == "get_peers") {
        auto info_hash = parseBDictionary<BString>(a, "info_hash");
        if (info_hash) {
            std::string info_hashstr = info_hash->value();
            // 问鸡哥与info_hash最接近的是什么，得到一个字符串组
            // 组成一个BOjbect，并加上token
            std::shared_ptr<BItem> get_peersRet;
            sendResonpse(tstr, get_peersRet);
        }
    } else if (qstr == "announce_peer") {
        // 检查token是否和之前发送给他的一样
        // 我自己记住对应的peer信息即可
    }
}

void DHT::handleResponse(std::shared_ptr<BDictionary> pBDictionary) {
    auto t = parseBDictionary<BString>(pBDictionary, "t");
    if (!t) return;
    std::string tstr = t->value();

    auto r = parseBDictionary<BDictionary>(pBDictionary, "r");
    if (!r) return;
    
    auto id = parseBDictionary<BString>(r, "id");
    if (!id) return;
    std::string idstr = id->value();
    // 告诉鸡哥这个是个好节点
    // tell ge ji this good
    // todo
    
    // 根据tstr找到响应的请求类型
    // 根据不同的请求类型处理
}

void DHT::handleError(std::shared_ptr<BDictionary>) {

}
