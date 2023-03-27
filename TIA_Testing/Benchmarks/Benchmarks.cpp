#include <Benchmarks/Benchmarks.h>  
#include <Core/TCore.h>
/*
This code is stolen from:
https://github.com/facebook/folly/blob/a3c67e006de0e32ecad1bc406ab74ca767e4caee/folly/Benchmark.h#L243s
*/

/**
 * Call doNotOptimizeAway(var) against variables that you use for
 * benchmarking but otherwise are useless. The compiler tends to do a
 * good job at eliminating unused variables, and this function fools
 * it into thinking var is in fact needed.
 */
#ifdef _MSC_VER

#pragma optimize("", off)

template <class T>
void doNotOptimizeAway(T&& datum) {
    datum = datum;
}

#pragma optimize("", on)

#elif defined(__clang__)

template <class T>
__attribute__((__optnone__)) void doNotOptimizeAway(T&& /* datum */) {}

#else

template <class T>
void doNotOptimizeAway(T&& datum) {
    asm volatile("" : "+r" (datum));
}

#endif


void Benchmark_TCore_LoadImages(const std::vector<std::filesystem::path>& paths, std::vector<cv::Mat>& outImages) {

    for (const std::filesystem::path& path : paths) {
        outImages.emplace_back(cv::imread(path.string()));
    }
}

void Benchmark_TCore_DetectImageContent(TCore& core, const std::vector<cv::Mat>& images, std::vector<std::vector<TItemSupport::DetectionResult>>& ret) {
        for (const cv::Mat& image : images) {
            ret.emplace_back(core.detectImageContent(image));
        }
}



static void Benchmark_TCore() {
    TCore core = TCore();
    core.loadCatalog("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");

    std::vector<std::filesystem::path> paths;
    std::filesystem::path imDir = "C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\screenshots\\raw";
        if (!imDir.is_absolute() || !std::filesystem::exists(imDir))
            return;

    std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(imDir);
    for (itr; itr != std::filesystem::end(itr); ++itr) {
        paths.emplace_back(*itr);
    }

    const int detectImageContentIterations = 20;
    for (int i = 0; i < detectImageContentIterations; i++) {

        std::vector<cv::Mat> outImages;

        outImages.reserve(paths.size());

        Benchmark_TCore_LoadImages(paths, outImages);

        std::vector<std::vector<TItemSupport::DetectionResult>> ret;
        ret.reserve(outImages.size());

        Benchmark_TCore_DetectImageContent(core, outImages, ret);
    }
}

//BENCHMARK(Benchmark_TCore);


//static void BM_StringCreation(benchmark::State& state) {
//    for (auto _ : state)
//        std::string empty_string;
//}
//// Register the function as a benchmark
//BENCHMARK(BM_StringCreation);
//
//// Define another benchmark
//static void BM_StringCopy(benchmark::State& state) {
//    std::string x = "hello";
//    for (auto _ : state)
//        std::string copy(x);
//}
//
//
//BENCHMARK(BM_StringCopy);


//// Define another benchmark
//static void bench_push_back(benchmark::State& state) {
//
//
//    while (state.KeepRunning()) {
//        std::vector<int> v;
//        v.reserve(1);
//        v.push_back(62);
//        doNotOptimizeAway(v.data());
//    }
//
//}
//
//BENCHMARK(bench_push_back);
//
//
//static void bench_create(benchmark::State& state) {
//    while (state.KeepRunning()) {
//        std::vector<int> v;
//        doNotOptimizeAway(v.data());
//        (void)v;
//
//    }
//
//}
//BENCHMARK(bench_create);
//
//
//static void bench_reserve(benchmark::State& state) {
//    while (state.KeepRunning()) {
//        std::vector<int> v;
//        v.reserve(1);
//        doNotOptimizeAway(v.data());
//    }
//
//}
//BENCHMARK(bench_reserve);
//


int main() {
        Benchmark_TCore();
}

//BENCHMARK_MAIN();