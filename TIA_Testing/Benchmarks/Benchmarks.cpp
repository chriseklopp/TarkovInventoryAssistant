#include <Benchmarks/Benchmarks.h>  



static void BM_StringCreation(benchmark::State& state) {
    for (auto _ : state)
        std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    for (auto _ : state)
        std::string copy(x);
}


BENCHMARK(BM_StringCopy);


// Define another benchmark
static void bench_push_back(benchmark::State& state) {
    std::vector<int> v;
    while (state.KeepRunning()) {

        v.push_back(69);
    }
}

BENCHMARK(bench_push_back);

BENCHMARK_MAIN();