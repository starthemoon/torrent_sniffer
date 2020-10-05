#pragma once

#include <memory>

#include "bencoding.h"

using bencoding::BDictionary;
using bencoding::BList;
using bencoding::BString;
using bencoding::BInteger;
using bencoding::BItem;
using bencoding::encode;
using bencoding::decode;

template<class T>
std::shared_ptr<T> parseBObject(std::shared_ptr<BItem> pBObject) {
    std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(pBObject);
    return ret;
}

template<class T>
std::shared_ptr<T> parseBDictionary(std::shared_ptr<BDictionary> pBDictionary, const std::string& key) {
    std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>((*pBDictionary)[BString::create(key)]);
    return ret;
}

