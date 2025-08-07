//
// Created by Arjun Varma on 23/07/25.
//

#ifndef THREADSAFETICKQUEUE_HPP
#define THREADSAFETICKQUEUE_HPP



#include <atomic>
#include <vector>
#include "MarketTick.hpp"
#include "ConcurrentQueue/concurrentqueue.hpp"

class ThreadSafeTickQueue {
private:
    moodycamel::ConcurrentQueue<MarketTick> queue;
    std::atomic<bool> doneProducing = false;

public:
    ThreadSafeTickQueue() = default;
    ThreadSafeTickQueue(const ThreadSafeTickQueue& other) = delete;
    ThreadSafeTickQueue& operator=(const ThreadSafeTickQueue& other) = delete;
    ThreadSafeTickQueue(ThreadSafeTickQueue&& other) = default;
    ThreadSafeTickQueue& operator=(ThreadSafeTickQueue&& other) = default;

    void enqueue(const MarketTick& tick) {
        queue.enqueue(tick);
    }

    bool tryDequeue(MarketTick& outTick) {
        return queue.try_dequeue(outTick);
    }

    bool tryDequeueMany(std::vector<MarketTick>& outTicks, size_t maxBatchSize = 64) {
        size_t count = 0;
        MarketTick tick;
        while (count < maxBatchSize && queue.try_dequeue(tick)) {
            outTicks.push_back(tick);
            count++;
        }
        return count > 0;
    }

    void markDone() {
        doneProducing.store(true);
    }

    bool isDone() const {
        return doneProducing.load() && queue.size_approx() == 0;
    }

    bool isEmpty() const {
        return queue.size_approx() == 0;
    }
};



#endif //THREADSAFETICKQUEUE_HPP
