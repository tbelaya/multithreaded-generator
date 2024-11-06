#pragma once
#include <random>

#include "thread_safe_queue.h"

/**
 * @class Producer
 * @brief A class that generates random integers and adds them to a queue.
 *
 * The Producer class creates random integers in a specified range
 * and pushes them into a provided thread-safe queue. It keeps track
 * of the number of elements to produce and can signal when the
 * production is complete.
 */
class Producer
{
   public:
    /**
     * @brief Constructs a Producer.
     *
     * @param queue Reference to the thread-safe queue where produced
     *              integers will be stored.
     * @param elements The number of elements to produce.
     * @param completed Reference to an atomic boolean to signal
     *                  completion of production.
     */
    Producer(core::ThreadSafeQueue<int>& queue, int elements, std::atomic_bool& completed)
        : m_elementsNr(elements)
        , m_queue(&queue)
        , m_completed(&completed)
        , m_generator(std::random_device{}())
        , m_distribution(1, m_elementsNr)
    {
    }

    /**
     * @brief Starts the production of random integers.
     *
     * This method generates random integers within the specified
     * range and pushes them to the associated thread-safe queue.
     * It runs until the specified number of elements has been produced
     * or the completion flag is set.
     */
    void produce();

   private:
    int m_elementsNr;  ///< The total number of elements to produce.
    core::ThreadSafeQueue<int>*
        m_queue;  ///< Pointer to the thread-safe queue for storing produced integers.
    std::atomic_bool* m_completed;           ///< Pointer to the completion flag.
    std::default_random_engine m_generator;  ///< Random number generator.
    std::uniform_int_distribution<int>
        m_distribution;  ///< Distribution for generating random integers.
};