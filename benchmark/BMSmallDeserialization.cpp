#include <sstream>
#include <benchmark/benchmark.h>

#define SO_USE_STD_OPERATORS
#include "soser.hpp"

struct A {
    int i = 1;
    float f = 1.0;
    std::string str = "hello";

    _PACK_THESE_(A, i, f, str);
};

static void BM_StructSerialization(benchmark::State& state) {
    std::string str("{\"i\": 10, \"f\": 10.5, \"str\": \"world\"}");
    A a;

    for (auto _ : state){
        str >> a;
    }
}

BENCHMARK(BM_StructSerialization);

BENCHMARK_MAIN();