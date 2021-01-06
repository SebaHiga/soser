#include <boost/archive/text_oarchive.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <benchmark/benchmark.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#define SO_USE_STD_OPERATORS
#include "soser.hpp"
#include <tser/tser.hpp>

struct A {
    int i = 1;
    float f = 1.0;
    std::string str = "hello";

    _PACK_THESE_(A, i, f, str);
};

static void BM_StructSerialization(benchmark::State& state) {
    A a;
    std::stringstream ss;
    for (auto _ : state) {
        ss << a;
    }
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

struct CEREAL {
    int i = 1;
    float f = 1.0;
    std::string str = "hello";

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( i, f, str ); // serialize things by passing them to the archive
    }
};

// Define another benchmark
static void BM_StructCereal(benchmark::State& state) {
    std::stringstream ss;
    CEREAL b;
    for (auto _ : state) {
        {
            cereal::JSONOutputArchive oarchive(ss);

            oarchive(b);
        }
        ss.str();
    }
}
BENCHMARK(BM_StructCereal);


struct BOOST{
    int i = 1;
    float f = 1.0;
    std::string str = "hello";

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version){
        ar & i;
        ar & f;
        ar & str;
    }
};

static void BM_StructBoost(benchmark::State& state) {
    std::stringstream ss;
    BOOST b;
    for (auto _ : state) {
        {
            boost::archive::text_oarchive ar(ss);

            ar << b;
        }

        ss.str();

    }
}
BENCHMARK(BM_StructBoost);

struct TSER {
    DEFINE_SERIALIZABLE(TSER, i, f, str);

    int i = 1;
    float f = 1.0;
    std::string str = "hello";
};

static void BM_StructTser(benchmark::State& state) {
    TSER tser;
    std::stringstream ss;
    for (auto _ : state) {
        ss << tser;

        ss.str();
    }
}
BENCHMARK(BM_StructTser);


BENCHMARK_MAIN();