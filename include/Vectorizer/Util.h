#pragma once

#include <stdio.h>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

template<typename T>
using unique = std::unique_ptr<T>;

template<typename T>
using shared = std::shared_ptr<T>;

template <typename T>
using weak = std::weak_ptr<T>;

template<typename T>
using List = std::vector<T>;

template<typename keyType, typename valueType, typename Pr = std::less<keyType>>
using Map = std::map<keyType, valueType, Pr>;

template<typename keyType, typename valueType, typename hasher = std::hash<keyType>>
using Dictionary = std::unordered_map<keyType, valueType, hasher>;

template<typename T>
using Set = std::unordered_set<T>;

#define LOG(M, ...) printf(M "\n", ##__VA_ARGS__)