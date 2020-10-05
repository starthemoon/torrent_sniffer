#include "gtest/gtest.h"
#include "krpc.h"

TEST(makeQuery, ping) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
        });
    auto ret = makeQuery("aa", krpcType::ping, a);
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe"));
}

TEST(makeQuery, findNode) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
            {BString::create("target"), BString::create("mnopqrstuvwxyz123456")},
        });
    auto ret = makeQuery("aa", krpcType::find_node, a);
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe"));
}

TEST(makeQuery, getPeers) {
    std::shared_ptr<BDictionary> a = 
        BDictionary::create({
            {BString::create("id"), BString::create("abcdefghij0123456789")},
            {BString::create("info_hash"), BString::create("mnopqrstuvwxyz123456")},
        });
    auto ret = makeQuery("aa", krpcType::get_peers, a);
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
    auto ret = makeQuery("aa", krpcType::announce_peer, a);
    EXPECT_EQ(ret, std::string("d1:ad2:id20:abcdefghij012345678912:implied_porti1e9:info_hash20:mnopqrstuvwxyz1234564:porti6881e5:token8:aoeusnthe1:q13:announce_peer1:t2:aa1:y1:qe"));
}


