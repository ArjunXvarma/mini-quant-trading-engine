//
// Created by Arjun Varma on 21/07/25.
//

#ifndef BENCHMARKRUNNER_HPP
#define BENCHMARKRUNNER_HPP



#include <memory>
#include <vector>

#include "Benchmark.hpp"
#include <vector>

class BenchmarkRunner {
private:
    std::vector<std::unique_ptr<Benchmark>> benchmarks;

public:
    void addBenchmark(std::unique_ptr<Benchmark> benchmark);
    void runAll() const;
};


#endif //BENCHMARKRUNNER_HPP
