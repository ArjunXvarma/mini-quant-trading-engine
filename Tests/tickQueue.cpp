//
// Created by Arjun Varma on 30/07/25.
//

#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "MarketDataHandler/ThreadSafeTickQueue.hpp"

MarketTick makeTick(double bid, double ask, const std::string& symbol = "AAPL") {
    MarketTick tick;
    tick.bidPrice = bid;
    tick.askPrice = ask;
    tick.symbol = symbol;
    return tick;
}

TEST(ThreadSafeTickQueueTest, BasicEnqueueDequeue) {
    ThreadSafeTickQueue queue;
    MarketTick tick = makeTick(100.0, 102.0);

    queue.enqueue(tick);

    MarketTick out;
    ASSERT_TRUE(queue.tryDequeue(out));
    EXPECT_EQ(out.bidPrice, 100.0);
    EXPECT_EQ(out.askPrice, 102.0);
    EXPECT_EQ(out.symbol, "AAPL");

    // Now the queue should be empty
    EXPECT_TRUE(queue.isEmpty());
}

TEST(ThreadSafeTickQueueTest, TryDequeueMany) {
    ThreadSafeTickQueue queue;

    for (int i = 0; i < 10; ++i) {
        queue.enqueue(makeTick(90 + i, 91 + i));
    }

    std::vector<MarketTick> batch;
    bool result = queue.tryDequeueMany(batch, 5);

    ASSERT_TRUE(result);
    ASSERT_EQ(batch.size(), 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(batch[i].bidPrice, 90 + i);
        EXPECT_EQ(batch[i].askPrice, 91 + i);
    }
}

TEST(ThreadSafeTickQueueTest, MarkDoneAndIsDone) {
    ThreadSafeTickQueue queue;
    queue.enqueue(makeTick(100.0, 101.0));

    EXPECT_FALSE(queue.isDone());

    MarketTick out;
    ASSERT_TRUE(queue.tryDequeue(out));
    EXPECT_TRUE(queue.isEmpty());

    queue.markDone();
    EXPECT_TRUE(queue.isDone());
}

TEST(ThreadSafeTickQueueTest, MultiThreadedProducerConsumer) {
    ThreadSafeTickQueue queue;
    constexpr int tickCount = 1000;
    std::atomic<int> consumedCount = 0;

    // Producer thread
    std::thread producer([&]() {
        for (int i = 0; i < tickCount; ++i) {
            queue.enqueue(makeTick(90 + i, 91 + i));
        }
        queue.markDone();
    });

    // Consumer thread
    std::thread consumer([&]() {
        while (!queue.isDone()) {
            MarketTick tick;
            if (queue.tryDequeue(tick)) {
                consumedCount++;
            } else {
                std::this_thread::yield();
            }
        }

        // Drain any final ticks after done
        MarketTick tick;
        while (queue.tryDequeue(tick)) {
            consumedCount++;
        }
    });

    producer.join();
    consumer.join();

    ASSERT_EQ(consumedCount, tickCount);
    EXPECT_TRUE(queue.isDone());
    EXPECT_TRUE(queue.isEmpty());
}
