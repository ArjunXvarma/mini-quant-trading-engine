import pandas as pd
from src.GenGraph import GenGraph

def getLatencies(filename: str):
    df = pd.read_csv(filename)
    return df['latency_ns'].tolist()

if __name__ == '__main__':
    # marketDataGraphs = GenGraph(getLatencies('Tests/results/performance/MarketDataLatencies.csv'), benchmark_name="MarketDataBenchmark")
    # marketDataGraphs.generateAll(save=True)

    # orderMatchingGraphs = GenGraph(getLatencies('Tests/results/performance/OrderMatchingLatencies.csv'), benchmark_name="OrderMatchingBenchmark")
    # orderMatchingGraphs.generateAll(save=True)

    # strategyLatencies = GenGraph(getLatencies('Tests/results/performance/StrategyLatencies.csv'), benchmark_name="StrategyBenchmark")
    # strategyLatencies.generateAll(save=True)

    orderBookLatencies = GenGraph(getLatencies('Performance/results/performance/OrderbookLatencies.csv'), benchmark_name="OrderbookBenchmark")
    orderBookLatencies.generateAll(save=True)
