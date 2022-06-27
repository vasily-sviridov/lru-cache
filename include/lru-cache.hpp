#pragma once

#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>

namespace cache {
    namespace {
        using LinkMode = boost::intrusive::link_mode<
#ifdef NDEBUG
                boost::intrusive::normal_link
#else
                boost::intrusive::safe_link
#endif
        >;
        using LruListHook = boost::intrusive::list_base_hook<LinkMode>;
        using LruHashSetHook = boost::intrusive::unordered_set_base_hook<LinkMode>;

        template<class Key>
        class LruNode final : public LruListHook, public LruHashSetHook {
        public:
            explicit LruNode(Key &&key) : _key(std::move(key)) {}

            const Key &GetKey() const noexcept { return _key; }

            void SetKey(Key key) { _key = std::move(key); }

        private:
            Key _key;
        };

        template<class Key>
        const Key &GetKey(const LruNode<Key> &node) noexcept {
            return node.GetKey();
        }

        template<class T>
        const T &GetKey(const T &key) noexcept {
            return key;
        }
    }

    template<typename Type, typename Hash = std::hash<Type>,
            typename Equal = std::equal_to<Type>>
    class LruCache final {
    public:
        explicit LruCache(size_t max_size)
                : _buckets(max_size ? max_size : 1),
                  _map(BucketTraits(_buckets.data(), _buckets.size())) {}

        ~LruCache() {
            while (!_list.empty()) {
                ExtractNode(_list.begin());
            }
        }

        LruCache(LruCache &&lru) = default;

        LruCache(const LruCache &lru) = delete;

        LruCache &operator=(LruCache &&lru) = default;

        LruCache &operator=(const LruCache &lru) = delete;

        bool Put(const Type &key) {
            auto it = _map.find(key, _map.hash_function(), _map.key_eq());
            if (it != _map.end()) {
                _list.splice(_list.end(), _list, _list.iterator_to(*it));
                return false;
            }

            if (_map.size() == _buckets.size()) {
                auto node = ExtractNode(_list.begin());
                node->SetKey(key);
                InsertNode(std::move(node));
            } else {
                auto node = std::make_unique<LruNode<Type>>(Type(key));
                InsertNode(std::move(node));
            }

            return true;
        }

        bool Has(const Type &key) {
            auto it = _map.find(key, _map.hash_function(), _map.key_eq());
            if (it == _map.end()) return false;

            _list.splice(_list.end(), _list, _list.iterator_to(*it));
            return true;
        }

    private:
        using List = boost::intrusive::list<
                LruNode<Type>,
                boost::intrusive::constant_time_size<false>
        >;

        struct LruNodeHash : Hash {
            template<class NodeOrKey>
            auto operator()(const NodeOrKey &x) const {
                return Hash::operator()(GetKey(x));
            }
        };

        struct LruNodeEqual : Equal {
            template<class NodeOrKey1, class NodeOrKey2>
            auto operator()(const NodeOrKey1 &x, const NodeOrKey2 &y) const {
                return Equal::operator()(GetKey(x), GetKey(y));
            }
        };

        using Map = boost::intrusive::unordered_set<
                LruNode<Type>, boost::intrusive::constant_time_size<true>,
                boost::intrusive::hash<LruNodeHash>,
                boost::intrusive::equal<LruNodeEqual>>;

        using BucketTraits = typename Map::bucket_traits;
        using BucketType = typename Map::bucket_type;

        std::unique_ptr<LruNode<Type>> ExtractNode(typename List::iterator it) noexcept {
            std::unique_ptr<LruNode<Type>> ret(&*it);
            _map.erase(_map.iterator_to(*it));
            _list.erase(it);
            return ret;
        }

        void InsertNode(std::unique_ptr<LruNode<Type>> node) noexcept {
            if (!node) return;

            _map.insert(*node);
            _list.insert(_list.end(), *node);

            [[maybe_unused]] auto ignore = node.release();
        }

        std::vector<BucketType> _buckets;
        Map _map;
        List _list;
    };
}