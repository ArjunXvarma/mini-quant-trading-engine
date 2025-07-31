//
// Created by Arjun Varma on 23/07/25.
//

#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <string>
#include <iostream>

struct BenchmarkResult {
    std::string name;
    double avgLatency;
    double p95Latency;
    double p99Latency;
    double maxLatency;
    double throughput;
};

class Benchmark {
protected:
    std::vector<long long> latencies;

public:
    virtual ~Benchmark() = default;

    virtual std::string getName() const = 0;
    virtual BenchmarkResult runBenchmark() = 0;

    std::vector<long long> getLatencies() {
        return latencies;
    }
};

#endif //BENCHMARK_HPP
