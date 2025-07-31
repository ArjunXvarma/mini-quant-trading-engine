//
// Created by Arjun Varma on 25/07/25.
//

#include "../../include/Benchmark/MetricsExporter.hpp"

#include <fstream>

using json = nlohmann::json;

void MetricsExporter::exportToCSV(const std::string& filename, const std::vector<BenchmarkResult>& results) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for CSV export: " << filename << std::endl;
        return;
    }

    // Write CSV header
    outFile << "Name,Avg Latency (μs),P95 Latency (μs),P99 Latency (μs),Max Latency (μs),Throughput (ops/sec)\n";

    for (const auto& result : results) {
        outFile << result.name << ","
                << result.avgLatency << ","
                << result.p95Latency << ","
                << result.p99Latency << ","
                << result.maxLatency << ","
                << result.throughput << "\n";
    }

    outFile.close();
}

void MetricsExporter::exportToJSON(const std::string& filename, const std::vector<BenchmarkResult>& results) {
    json j;

    for (const auto& result : results) {
        j["benchmarks"].push_back({
            {"name", result.name},
            {"avg_latency_us", result.avgLatency},
            {"p95_latency_us", result.p95Latency},
            {"p99_latency_us", result.p99Latency},
            {"max_latency_us", result.maxLatency},
            {"throughput_ops_per_sec", result.throughput}
        });
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for JSON export: " << filename << std::endl;
        return;
    }

    outFile << std::setw(4) << j << std::endl;
    outFile.close();
}

void MetricsExporter::logToConsole(const std::vector<BenchmarkResult>& results) {
    std::cout << "================== BENCHMARK RESULTS ==================\n";
    for (const auto& result : results) {
        std::cout << "Benchmark: " << result.name << "\n"
                  << "  Avg Latency: " << result.avgLatency << " μs\n"
                  << "  P95 Latency: " << result.p95Latency << " μs\n"
                  << "  P99 Latency: " << result.p99Latency << " μs\n"
                  << "  Max Latency: " << result.maxLatency << " μs\n"
                  << "  Throughput: " << result.throughput << " ops/sec\n\n";
    }
    std::cout << "=======================================================\n";
}

void MetricsExporter::exportLatenciesToCSV(const std::vector<long long>& latencies,
                                            const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open latency CSV file: " << filename << std::endl;
        return;
    }

    file << "latency_ns\n";

    for (const auto& latency : latencies) {
        file << latency << "\n";
    }

    file.close();
}