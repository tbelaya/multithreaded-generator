#include <gtest/gtest.h>

#include <thread>

#include "thread_safe_queue.h"

// Test Case 1: tryPush and tryPop success
TEST(ThreadSafeQueue, PushAndPopSuccessTest)
{
    core::ThreadSafeQueue<int> queue(3);
    int value;

    // Pushing elements successfully
    EXPECT_TRUE(queue.tryPush(1));
    EXPECT_TRUE(queue.tryPush(2));
    EXPECT_TRUE(queue.tryPush(3));

    // Popping elements and verifying FIFO order
    EXPECT_TRUE(queue.tryPop(value));
    EXPECT_EQ(value, 1);
    EXPECT_TRUE(queue.tryPop(value));
    EXPECT_EQ(value, 2);
    EXPECT_TRUE(queue.tryPop(value));
    EXPECT_EQ(value, 3);
}

// Test Case 2: tryPush failure
TEST(ThreadSafeQueue, PushFailureTest)
{
    core::ThreadSafeQueue<int> queue(2);

    EXPECT_TRUE(queue.tryPush(1));
    EXPECT_TRUE(queue.tryPush(2));

    // Attempt to push an element when the queue is full
    EXPECT_FALSE(queue.tryPush(3));
}

// Test Case 3: tryPop failure
TEST(ThreadSafeQueue, PopFailureTest)
{
    core::ThreadSafeQueue<int> queue(2);

    int value;
    // Attempt to pop an element from an empty queue
    EXPECT_FALSE(queue.tryPop(value));
}

// Test Case 4: tryPush and tryPop until failure
TEST(ThreadSafeQueue, PushAndPopUntilFailureTest)
{
    core::ThreadSafeQueue<int> queue(2);
    int value;

    // Pushing more elements than the queue size is
    EXPECT_TRUE(queue.tryPush(1));
    EXPECT_TRUE(queue.tryPush(2));
    EXPECT_FALSE(queue.tryPush(3));

    // Popping elements even if the queue is empty
    EXPECT_TRUE(queue.tryPop(value));
    EXPECT_TRUE(queue.tryPop(value));
    EXPECT_FALSE(queue.tryPop(value));
}

// Test Case 5: concurrent push test
TEST(ThreadSafeQueue, ConcurrentPushTest)
{
    const size_t queueSize = 10;
    core::ThreadSafeQueue<int> queue(queueSize);
    const size_t numThreads = 5;
    const size_t numPushesPerThread = 20;
    std::vector<std::thread> threads;
    std::atomic<size_t> successfulPushes(false);

    // Create and launch producer threads
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(
            [&]()
            {
                for (size_t j = 0; j < numPushesPerThread; ++j)
                {
                    if (queue.tryPush(j))
                    {
                        successfulPushes++;
                    }
                }
            });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    // The number of successful pushes should not exceed the queue size
    EXPECT_LE(successfulPushes.load(), queueSize);
}

// Test Case 6: concurrent pop test
TEST(ThreadSafeQueue, ConcurrentPopTest)
{
    const size_t queueSize = 10;
    core::ThreadSafeQueue<int> queue(queueSize);

    for (int i = 0; i < queueSize; ++i)
    {
        EXPECT_TRUE(queue.tryPush(i));
    }

    const size_t numThreads = 5;
    std::vector<std::thread> threads;
    std::atomic<size_t> successfulPops(false);

    // Create and launch consumer threads
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(
            [&]()
            {
                int value;
                while (queue.tryPop(value))
                {
                    successfulPops++;
                }
            });
    }

    // Join threads
    for (auto& thread : threads)
    {
        thread.join();
    }

    // The number of successful pops should match the queue size
    EXPECT_EQ(successfulPops.load(), queueSize);
}

// Test Case 7: empty queue under concurrent pop test
TEST(ThreadSafeQueue, EmptyQueueConcurrentPopTest)
{
    core::ThreadSafeQueue<int> queue(5);
    const size_t numThreads = 5;
    std::vector<std::thread> threads;
    std::atomic<size_t> failedPops(false);

    // Create and launch consumer threads
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(
            [&]()
            {
                int value;
                if (!queue.tryPop(value))
                {
                    failedPops++;
                }
            });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    // All pops should fail
    EXPECT_EQ(failedPops.load(), numThreads);
}

// Test Case 8: full queue under concurrent push test
TEST(ThreadSafeQueue, FullQueueConcurrentPushTest)
{
    const size_t queueSize = 5;
    core::ThreadSafeQueue<int> queue(queueSize);

    for (int i = 0; i < queueSize; ++i)
    {
        EXPECT_TRUE(queue.tryPush(i));
    }

    const size_t numThreads = 5;
    std::vector<std::thread> threads;
    std::atomic<size_t> failedPushes(false);

    // Create and launch producer threads
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(
            [&]()
            {
                if (!queue.tryPush(10))
                {
                    failedPushes++;
                }
            });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    // All pushes should fail
    EXPECT_EQ(failedPushes.load(), numThreads);
}