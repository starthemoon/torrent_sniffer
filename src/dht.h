#pragma onece

#include <string>
#include <bencoding.h>
#include <atomic>
#include <unordered_map>
#include <mutex>

#include "transaction.h"
#include "krpc.h"
#include "routingtable.h"


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

    std::shared_ptr<TransactionManager> transMan;
    std::shared_ptr<RoutingTable> rTable;

    // 将传入的事务id、查询类型以及查询参数组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeQuery(const std::string& tid, enum queryType type, std::shared_ptr<BItem> a);

    // 将传入的事务id、响应数据组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeResponse(const std::string& tid, std::shared_ptr<BItem> r);

    // 将传入的事务id、错误数据组合成一个完整的BDictionary
    std::shared_ptr<BDictionary> makeError(const std::string& tid, std::shared_ptr<BItem> e);

};

