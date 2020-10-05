#include "transaction.h"

std::string TransactionManager::getNextID() {
    int num;
    {
        std::lock_guard<std::mutex> lk(mutex);
        cursor = (cursor + 1) % maxCursor;
        num = cursor;
    }
    return std::string(static_cast<const char*>(static_cast<const void*>(&num)), sizeof(num));
}

void TransactionManager::newTrans(const std::string& tid, std::shared_ptr<bencoding::BDictionary> query) {
    std::shared_ptr<Transaction> ret(new Transaction(tid, query));
    std::lock_guard<std::mutex> lk(mutex);
    map[tid] = ret;
}

std::shared_ptr<Transaction> TransactionManager::operator[](std::string tid) {
    std::lock_guard<std::mutex> lk(mutex);
    return map[tid];
} 
