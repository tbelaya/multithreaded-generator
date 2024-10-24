#include "cv_based_threading.h"

#include <condition_variable>
#include <format>
#include <iostream>
#include <mutex>
#include <random>

std::default_random_engine
    g_generator(  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        std::random_device{}());
std::uniform_int_distribution<int>
    g_distribution;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

std::mutex g_producerMtx;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
std::condition_variable g_cv;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

std::atomic_int g_order = 1;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
long long g_startTime;        // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

long long getCurrentTimeInMicroseconds()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

void initializeDistribution(int to)
{
    g_distribution = std::uniform_int_distribution<int>(1, to);
}

void initializeStartTime()
{
    g_startTime = getCurrentTimeInMicroseconds();
}

void produce(ThreadSafeQueue<int>& queue, int elements, std::atomic_bool& completed)
{
    while (!completed.load())
    {
        if (!queue.tryPush(g_distribution(g_generator)))
        {
            // The producer thread will wait on the CV until one of the consumers signals that space
            // has been freed in the queue, allowing the producer to continue producing items
            // safely.
            std::unique_lock<std::mutex> lock(g_producerMtx);
            g_cv.wait(lock);
        }
    }
    std::cout << "CV-based producer finished task.\n";
}

void consume(ThreadSafeQueue<int>& queue,
             std::vector<NumberInfo>& storage,
             int elements,
             std::atomic_bool& completed)
{
    int randValue{};
    while (!completed)
    {
        if (queue.tryPop(randValue))
        {
            int index = randValue - 1;
            size_t expected = 0;
            if (!completed &&  // Check if the generated number is already present in the
                               // storage. Do it in a thread-safe manner using the atomic
                               // operation compare_exchange_strong.
                storage[index].m_order.compare_exchange_strong(expected, g_order))
            {
                // Calculate time it took to generate the value
                auto endTime = getCurrentTimeInMicroseconds();
                auto timeTaken = endTime - g_startTime;

                // Save the generated number
                storage[index].m_order = g_order++;
                storage[index].m_generationTime = timeTaken;

                std::string output =
                    std::format("number = {:05}, order = {:05}, generation_time = {:010}\n",
                                randValue, storage[index].m_order.load(), timeTaken);
                std::cout << output;

                if (g_order == elements + 1)
                {
                    // All the numbers are generated
                    completed.store(true);
                    g_cv.notify_all();
                    break;
                }

                g_startTime = getCurrentTimeInMicroseconds();
            }
            g_cv.notify_one();
        }
    }
    std::cout << "CV-based consumer finished task.\n";
}