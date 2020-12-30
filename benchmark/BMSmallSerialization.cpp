#include <boost/archive/text_oarchive.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <benchmark/benchmark.h>
#include <cereal/archives/json.hpp>
#include <sstream>
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
    for (auto _ : state) {
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oarchive(ss); // Create an output archive

            CEREAL b;
            oarchive(b); // Write the data to the archive
            ss.str();
        } // archive goes out of scope, ensuring all contents are flushed
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
    for (auto _ : state) {
        std::stringstream ss;
        BOOST b;

        {
            boost::archive::text_oarchive ar(ss);

            ar << b;
        }

        ss.str();

    }
}
BENCHMARK(BM_StructBoost);


BENCHMARK_MAIN();