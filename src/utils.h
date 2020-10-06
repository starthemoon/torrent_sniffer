#pragma once

#include <stdlib.h>
#include <time.h>
#include <memory>

#include "bencoding.h"

using bencoding::BDictionary;
using bencoding::BList;
using bencoding::BString;
using bencoding::BInteger;
using bencoding::BItem;
using bencoding::encode;
using bencoding::decode;

/*
    踩坑，模版函数的声明和定义必须放在同一个文件中，否则会出现找不到定义的问题
    由于链接时会做去重的操作，所以不用担心重复定义的问题
*/
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

/*
    非模版函数，需要将声明和定义分开放，不然会出现重复定义的问题
*/
std::string randomString(int len);

