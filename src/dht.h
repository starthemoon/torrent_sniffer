#pragma onece

#include <string>
#include <bencoding.h>
#include <atomic>
#include <unordered_map>
#include <mutex>

#include "transaction.h"
#include "krpc.h"
#include "routingtable.h"
#include "token.h"
#include "infoHash.h"


using bencoding::BDictionary;
using bencoding::BList;
using bencoding::BString;
using bencoding::BInteger;
using bencoding::BItem;
using bencoding::encode;
using bencoding::decode;

// 作为DHT，具有响应其它DHT节点消息的功能。
// 同时，也具有接收本机上其它程序发送DHT消息的功能。
class DHT {
public:

    enum class queryType {
        ping,
        find_node,
        get_peers,
        announce_peer,
    };

    static std::vector<std::string> queryTypeName;

    static std::unordered_map<std::string, enum queryType> nameQueryType;

    DHT();

    // 响应一条来自外界的DHT查询数据
    void handleQuery(std::shared_ptr<BDictionary>);

    // 响应一条来自外界的DHT响应数据
    void handleResponse(std::shared_ptr<BDictionary>);

    // 响应一条来自外界的DHT错误数据
    void handleError(std::shared_ptr<BDictionary>);

    // 调用者发送一条DHT查询消息，需要传入查询类型和查询参数
    void sendQuery(enum queryType type, std::shared_ptr<BItem> a, const std::string id);

    // 调用者发送一条DHT响应消息，需要传入事务ID和响应数据
    void sendResonpse(const std::string& tid, std::shared_ptr<BItem> r);

    // 调用者发送一条DHT错误消息，需要传入事务ID和错误数据
    void sendError(const std::string& tid, std::shared_ptr<BItem> e);

    std::shared_ptr<TransactionManager> getTransMan() { return transMan; }

private:

    // 事务管理器，用于管理本地发起dht事务。
    // 一个事务包括：本地发起的查询请求，与该请求相对的请求
    std::shared_ptr<TransactionManager> transMan;

    // 路由表，用于查询160位命名空间下与node_id或与info_hash最接近的k个dht nodes信息
    std::shared_ptr<RoutingTable> rTable;

    // token管理器，关联IP地址和tokenstr。
    // 传入IP地址，创建一个token。传入IP地址和token，告知调用者这个组合是否合法
    std::shared_ptr<TokenManager> tokenMan;

    std::shared_ptr<InfoHashManager> infoHashMan;

    // 将传入的事务id、查询类型以及查询参数组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeQuery(const std::string& tid, enum queryType type, std::shared_ptr<BItem> a);

    // 将传入的事务id、响应数据组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeResponse(const std::string& tid, std::shared_ptr<BItem> r);

    // 将传入的事务id、错误数据组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeError(const std::string& tid, std::shared_ptr<BItem> e);

};

