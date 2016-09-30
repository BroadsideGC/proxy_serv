

#ifndef lru_cache_h
#define lru_cache_h

#include <stdio.h>
#include <list>
#include <map>
#include <exception>

template<typename key_t, typename value_t>
class lru_cache {

public:
    typedef std::pair<key_t, value_t> pair;
    typedef typename std::list<pair>::iterator iterator;

    lru_cache(size_t max_capacity = 512) : capacity(max_capacity) {}

    size_t size() {
        return list.size();
    }

    const value_t &get(const key_t &key) {
        auto it = iterators.find(key);
        if (it == iterators.end()) {
            throw std::runtime_error("Item with given key not found in cache!");
        } else {
            iterator it_on_queue = it->second;
            list.splice(list.begin(), list, it_on_queue);
            return it_on_queue->second;
        }
    }

    void put(const key_t &key, const value_t &value) {
        auto it = iterators.find(key);
        if (it != iterators.end()) {
            list.erase(it->second);
            iterators.erase(it);
        }

        list.push_front({key, value});
        iterators[key] = list.begin();

        if (size() > capacity) {
            auto last = list.back();
            iterators.erase(last.first);
            list.pop_back();
        }
    }

    bool contains(const key_t &key) {
        return iterators.find(key) != iterators.end();
    }


private:
    size_t capacity;
    std::list<pair> list;
    std::map<key_t, iterator> iterators;
};

#endif /* lru_cache_h */
