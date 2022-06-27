#include <lru-cache.hpp>
#include <effolkronium/random.hpp>
#include <gtest/gtest.h>
#include <random>

TEST(LruContainer, LruUnitTests) {
    using namespace cache;
    using Random = effolkronium::random_static;


    constexpr static uint32_t lru_size = 50000;
    LruCache<std::uint64_t> lru(lru_size);

    for (int i = 0; i < lru_size; ++i) {
        auto random = Random::get();
        lru.Put(random);
        ASSERT_TRUE(lru.Has(random));
    }
}