//
// Created by Arjun Varma on 23/07/25.
//

#ifndef METRICSEXPORTER_HPP
#define METRICSEXPORTER_HPP

#include <vector>
#include <string>

#include "nlohmann/json.hpp"
#include "Benchmark.hpp"

class MetricsExporter {
public:
    static void exportToCSV(const std::string& filename, const std::vector<BenchmarkResult>& results);
    static void exportToJSON(const std::string& filename, const std::vector<BenchmarkResult>& results);
    static void logToConsole(const std::vector<BenchmarkResult>& results);
    static void exportLatenciesToCSV(const std::vector<long long>& latencies, const std::string& filename);
};

#endif //METRICSEXPORTER_HPP
