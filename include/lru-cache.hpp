#pragma once

#include <list>
#include <unordered_map>

namespace cache {

template <typename Type>
class LruCache {
public:
    explicit LruCache(std::size_t max_size) : _max_size(max_size) {};

    bool Put(const Type& key) {
        auto currentElementIterator = _map.find(key);
        if (currentElementIterator != _map.end()) {
            auto list_it = currentElementIterator->second;
            _list.splice(_list.end(), _list, list_it);
            return false;
        }

        if (_list.size() == _max_size) {
            Type last = _list.front();
            auto node = _map.extract(last);

            _list.front() = key;
            _list.splice(_list.end(), _list, _list.begin());
            node.key() = key;
            node.mapped() = --_list.end();
            _map.insert(std::move(node));
            return true;
        }

        _map[key] = _list.insert(_list.end(), key);
        return true;
    }

    bool Has(const Type& key) {
        auto currentElementIterator = _map.find(key);

        if (currentElementIterator == _map.end()) {
            return false;
        }

        auto list_it = currentElementIterator->second;

        _list.splice(_list.end(), _list, list_it);
        return true;
    }

private:
    std::list<Type> _list;
    std::unordered_map<Type, typename std::list<Type>::iterator> _map;
    std::size_t _max_size;
};

}; // namespace cache
