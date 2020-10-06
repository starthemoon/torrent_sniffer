#include "infoHash.h"
#include <algorithm>

InfoHashManager::InfoHashManager()
    : map(), mutex() {}

std::vector<std::string> InfoHashManager::getPeers(const std::string& infoHash) {
    std::vector<std::string> ret;
    std::lock_guard<std::mutex> lk(mutex);
    if (map.find(infoHash) != map.end()) {
        ret = map[infoHash];
    }
    return ret;
}

void InfoHashManager::addPeer(const std::string& infoHash, const std::string& peerInfo) {
    std::lock_guard<std::mutex> lk(mutex);
    if (map.find(infoHash) != map.end()) {
        if (std::find(map[infoHash].begin(), map[infoHash].end(), peerInfo) != map[infoHash].end())
            map[infoHash].push_back(peerInfo);
    } else {
        map[infoHash] = {peerInfo};
    }
}

