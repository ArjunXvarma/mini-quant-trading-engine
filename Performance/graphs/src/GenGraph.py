import matplotlib.pyplot as plt
import numpy as np
import os

class GenGraph:
    def __init__(self, latencies: list, benchmark_name: str = "latency"):
        self.latencies = latencies
        self.benchmark_name = benchmark_name

    def latencyHistogram(self, save: bool = False):
        plt.figure(figsize=(10, 6))
        plt.hist(self.latencies, bins=100, color='skyblue', edgecolor='black')
        plt.title(f"{self.benchmark_name} Latency Histogram")
        plt.xlabel("Latency (ns)")
        plt.ylabel("Frequency")
        plt.grid(True)
        if save:
            self._save_fig("latency_histogram")
        else:
            plt.show()

    def boxPlot(self, save: bool = False):
        plt.figure(figsize=(8, 4))
        plt.boxplot(self.latencies, vert=False, patch_artist=True,
                    boxprops=dict(facecolor='lightblue', color='black'),
                    medianprops=dict(color='red'))
        plt.title(f"{self.benchmark_name} Box Plot")
        plt.xlabel("Latency (ns)")
        plt.grid(True)
        if save:
            self._save_fig("box_plot")
        else:
            plt.show()

    def latencyCDF(self, save: bool = False):
        plt.figure(figsize=(10, 6))
        sorted_latencies = np.sort(self.latencies)
        cdf = np.arange(len(sorted_latencies)) / float(len(sorted_latencies))
        plt.plot(sorted_latencies, cdf, color='blue')
        plt.title(f"{self.benchmark_name} CDF")
        plt.xlabel("Latency (ns)")
        plt.ylabel("Cumulative Probability")
        plt.grid(True)
        if save:
            self._save_fig("cdf")
        else:
            plt.show()

    def logScaleHistogram(self, save: bool = False):
        plt.figure(figsize=(10, 6))
        plt.hist(self.latencies, bins=100, log=True, color='orange', edgecolor='black')
        plt.title(f"{self.benchmark_name} Log-Scale Histogram")
        plt.xlabel("Latency (ns)")
        plt.ylabel("Log Frequency")
        plt.grid(True)
        if save:
            self._save_fig("log_histogram")
        else:
            plt.show()

    def generateAll(self, save: bool = False):
        self.latencyHistogram(save)
        self.boxPlot(save)
        self.latencyCDF(save)
        self.logScaleHistogram(save)

    def _save_fig(self, fig_type: str):
        dir_path = f"Tests/results/graphs"
        os.makedirs(dir_path, exist_ok=True)
        plt.savefig(f"{dir_path}/{self.benchmark_name}_{fig_type}.png", dpi=300)
        plt.close()
