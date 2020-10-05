#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <atomic>
#include <mutex>

#include "bencoding.h"

struct Transaction {
    std::string tid;
    std::shared_ptr<bencoding::BDictionary> query;
    std::atomic<bool> flag;

    Transaction(std::string id, std::shared_ptr<bencoding::BDictionary> pTrans)
        : tid(id), query(pTrans),
            flag(false) {}
};


// 可以调用newTrans，传入BOject并构造Transaction
// 可以调用operator[string]，获得指向Transaction的指针
class TransactionManager {

public:
    TransactionManager(int maxNum=100000) : cursor(0), maxCursor(maxNum) {}

    std::string getNextID();

    void newTrans(const std::string& tid, std::shared_ptr<bencoding::BDictionary> query);

    std::shared_ptr<Transaction> operator[](std::string tid);

    int getCursor() const { 
        std::lock_guard<std::mutex> lk(mutex);
        return cursor; 
    }

    int size() const {
        std::lock_guard<std::mutex> lk(mutex);
        return map.size();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Transaction>> map;
    mutable std::mutex mutex;
    int cursor;
    int maxCursor;


};