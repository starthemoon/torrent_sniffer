#pragma

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

class InfoHashManager {

public:

    InfoHashManager();

    // 输入infohash，返回其对应的Peer信息——IP+Port
    std::vector<std::string> getPeers(const std::string& infoHash);

    // 输入infohash以及Peer信息——IP+Port，为infohash添加一个Peer
    void addPeer(const std::string& infoHash, const std::string& peerInfo);

private:
    std::unordered_map<std::string, std::vector<std::string>> map;
    std::mutex mutex;

};