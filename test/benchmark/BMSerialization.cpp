#include <benchmark/benchmark.h>
#include "sopack.hpp"

struct A {
  int i = 1;
  float f = 1.0;
  std::string str = "hello";

  _PACK_THESE_(A, i, f, str);
};

static void BM_StructSerialization(benchmark::State& state) {
  A a;
  for (auto _ : state)
    a._so_serialize();
}
// Register the function as a benchmark
BENCHMARK(BM_StructSerialization);

// Define another benchmark
static void BM_StructSerializationRaw(benchmark::State& state) {
  for (auto _ : state) {
    std::string str;
    str.append("{")
      .append("\"i\": ").append(std::to_string(1))
      .append(", \"f\": ").append(std::to_string(1.0))
      .append(", \"str\": ").append(std::string("\"hello\""))
      .append("}");
  }
}
BENCHMARK(BM_StructSerializationRaw);

BENCHMARK_MAIN();