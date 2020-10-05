#include <queue>
#include <string>
#include <functional>

#include "gtest/gtest.h"
#include "dht.h"
// #include "krpc.h"

std::queue<std::string> queue;
std::function<void(const std::string&)> KRPC::messageSender = [](const std::string& message){
    queue.push(message);
};

std::function<void(std::shared_ptr<BDictionary> pBObject)> KRPC::queryHandler = [](std::shared_ptr<BDictionary>) {};
std::function<void(std::shared_ptr<BDictionary> pBObject)> KRPC::responseHandler = [](std::shared_ptr<BDictionary>) {};
std::function<void(std::shared_ptr<BDictionary> pBObject)> KRPC::errorHandler = [](std::shared_ptr<BDictionary>) {};

DHT dht;

TEST(makeQuery, ping) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
        });
    dht.sendQuery(DHT::queryType::ping, a, "aa");
    auto ret = queue.front();
    queue.pop();
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe"));
}

TEST(makeQuery, findNode) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
            {BString::create("target"), BString::create("mnopqrstuvwxyz123456")},
        });
    dht.sendQuery(DHT::queryType::find_node, a, "aa");
    auto ret = queue.front();
    queue.pop();
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe"));
}

TEST(makeQuery, getPeers) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
            {BString::create("info_hash"), BString::create("mnopqrstuvwxyz123456")},
        });
    dht.sendQuery(DHT::queryType::get_peers, a, "aa");
    auto ret = queue.front();
    queue.pop();
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij01234567899:info_hash20:mnopqrstuvwxyz123456e1:q9:get_peers1:t2:aa1:y1:qe"));
}

TEST(makeQuery, announcePeer) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
            {BString::create("implied_port"), BInteger::create(1)},
            {BString::create("info_hash"), BString::create("mnopqrstuvwxyz123456")},
            {BString::create("port"), BInteger::create(6881)},
            {BString::create("token"), BString::create("aoeusnth")},
        });
    dht.sendQuery(DHT::queryType::announce_peer, a, "aa");
    auto ret = queue.front();
    queue.pop();
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij012345678912:implied_porti1e9:info_hash20:mnopqrstuvwxyz1234564:porti6881e5:token8:aoeusnthe1:q13:announce_peer1:t2:aa1:y1:qe"));
}

TEST(TransMan, cursorAndSize) {
    EXPECT_EQ(dht.getTransMan()->getCursor(), 0);
    EXPECT_EQ(dht.getTransMan()->size(), 1);
    EXPECT_TRUE((*dht.getTransMan())["aa"] != nullptr);
}
