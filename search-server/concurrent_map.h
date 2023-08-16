#pragma once
#include <string>
#include <mutex>
#include <map>
#include <vector>

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

    struct Access {
        Access(const Key& key, std::map<Key, Value>& local_map, std::vector<std::mutex>& mut, const size_t id) : guard(mut[id]), ref_to_value(local_map[key])  {}
        std::lock_guard<std::mutex> guard;
        Value& ref_to_value;
    };

    explicit ConcurrentMap(size_t bucket_count) : vector_maps(bucket_count),mut(bucket_count)  {}

    Access operator[](const Key& key) {
        size_t id = static_cast<size_t>(key) % mut.size();
        return Access(key, vector_maps[id], mut, id);
    }

    std::map<Key, Value> BuildOrdinaryMap() {
        std::map<Key, Value> result;
        size_t id = 0;
        for(const auto& mp : vector_maps) {
            std::lock_guard g(mut[id++]);
            result.insert(mp.begin(), mp.end());
        }
        return result;
    }

private:
    std::vector<std::map<Key, Value>> vector_maps;
    std::vector<std::mutex> mut;
};