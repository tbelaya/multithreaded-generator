#pragma once
#include <unordered_map>

#include "thread_safe_queue.h"

/**
 * @struct NumberInfo
 * @brief Structure to hold information about a number.
 *
 * This structure contains metadata related to a produced number,
 * including its order and the time it was generated.
 */

struct NumberInfo
{
    std::atomic<size_t> m_order = 0;  ///< The order in which the number was generated.
    long long m_generationTime =
        0;  ///< The time in microseconds that was taken to generate the number.
};

/**
 * @class Consumer
 * @brief A class that consumes integers from a queue and processes them.
 *
 * The Consumer class retrieves integers from a specified thread-safe
 * queue, records their generation time, and maintains an order
 * of processing. It continues until the specified number of
 * elements has been consumed or the completion flag is set.
 */
class Consumer
{
   public:
    /**
     * @brief Constructs a Consumer.
     *
     * @param queue Reference to the thread-safe queue from which integers
     *              will be consumed.
     * @param storage Reference to a vector of NumberInfo to store
     *                information about consumed numbers.
     * @param elements The number of elements to consume.
     * @param completed Reference to an atomic boolean to signal
     *                  completion of consumption.
     */
    Consumer(ThreadSafeQueue<int>& queue,
             std::vector<NumberInfo>& storage,
             int elements,
             std::atomic_bool& completed)
        : m_queue(&queue), m_storage(&storage), m_elementsNr(elements), m_completed(&completed)
    {
    }

    /**
     * @brief Starts the consumption of integers from the queue.
     *
     * This method retrieves integers from the associated thread-safe
     * queue, records their generation time, and updates the order
     * of consumption. It runs until the specified number of elements
     * has been consumed or the completion flag is set.
     */
    void consume();

    /**
     * @brief Sets the start time for generation.
     *
     * This static method can be called to initialize the start
     * time for generation tracking.
     */
    static void setStartTime();

   private:
    /**
     * @brief Gets the current time in microseconds.
     *
     * This static method retrieves the current time in microseconds,
     * useful for timing operations.
     *
     * @return Current time in microseconds.
     */
    static long long getCurrentTimeInMicroseconds();

   private:
    int m_elementsNr;                         ///< The total number of elements to consume.
    inline static long long m_startTime = 0;  ///< Start time for consumption tracking.
    ThreadSafeQueue<int>* m_queue;  ///< Pointer to the thread-safe queue for retrieving integers.
    std::vector<NumberInfo>*
        m_storage;                  ///< Pointer to the storage vector for consumed number info.
    std::atomic_bool* m_completed;  ///< Pointer to the completion flag.
    inline static std::atomic<size_t> m_order = 1;  ///< Atomic counter for order of consumption.
};