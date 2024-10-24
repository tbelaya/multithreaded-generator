#pragma once
#include <random>

#include "thread_safe_queue.h"

class Producer
{
   public:
    Producer(ThreadSafeQueue<int>& queue, int elements, std::atomic_bool& completed)
        : m_queue(&queue)
        , m_elementsNr(elements)
        , m_completed(&completed)
        , m_generator(std::random_device{}())
        , m_distribution(1, m_elementsNr)
    {
    }

    void produce();

   private:
    int m_elementsNr;
    std::atomic_bool* m_completed;
    ThreadSafeQueue<int>* m_queue;
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_distribution;
};