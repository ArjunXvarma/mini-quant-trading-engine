//
// Created by Arjun Varma on 21/07/25.
//

#include "../../include/Benchmark/BenchmarkRunner.hpp"

void BenchmarkRunner::addBenchmark(std::unique_ptr<Benchmark> benchmark) {
    benchmarks.emplace_back(std::move(benchmark));
}

void BenchmarkRunner::runAll() const {
    for (const auto& benchmark : benchmarks) {
        BenchmarkResult result = benchmark->runBenchmark();
        std::cout << "[" << result.name << "]\n";
        std::cout << "Avg Latency: " << result.avgLatency << " μs\n";
        std::cout << "P95 Latency: " << result.p95Latency << " μs\n";
        std::cout << "P99 Latency: " << result.p99Latency << " μs\n";
        std::cout << "Max Latency: " << result.maxLatency << " μs\n";
        std::cout << "Throughput: " << result.throughput << " ops/sec\n\n";
    }
}