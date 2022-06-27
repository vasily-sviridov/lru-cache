#include <lru-cache.hpp>
#include <benchmark/benchmark.h>

namespace {
    constexpr auto kElementsCount = 1000;

    template <class Lru>
    Lru FillLru(unsigned elements_count) {
        Lru lru(kElementsCount);
        for (unsigned i = 0; i < elements_count; ++i) {
            lru.Put(i);
        }

        return lru;
    }

    template<class Lru>
    void LruPut(benchmark::State &state) {
        for (auto _: state) {
            auto lru = FillLru<Lru>(kElementsCount);
            benchmark::DoNotOptimize(lru);
        }
    }

    BENCHMARK_TEMPLATE(LruPut, cache::LruCache<unsigned>);

    template <class Lru>
    void LruHas(benchmark::State& state) {
        auto lru = FillLru<Lru>(kElementsCount);
        for (auto _ : state) {
            for (unsigned i = 0; i < kElementsCount; ++i) {
                benchmark::DoNotOptimize(lru.Has(i));
            }
        }
    }

    BENCHMARK_TEMPLATE(LruHas, cache::LruCache<unsigned>);

}

BENCHMARK_MAIN();