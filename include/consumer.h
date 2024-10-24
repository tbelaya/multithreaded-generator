#pragma once
#include <unordered_map>

#include "thread_safe_queue.h"

struct NumberInfo
{
    std::atomic<size_t> m_order = 0;
    long long m_generationTime = 0;
};

class Consumer
{
   public:
    Consumer(ThreadSafeQueue<int>& queue,
             std::vector<NumberInfo>& storage,
             int elements,
             std::atomic_bool& completed)
        : m_queue(&queue), m_storage(&storage), m_elementsNr(elements), m_completed(&completed)
    {
    }

    void consume();
    static void setStartTime();

   private:
    static long long getCurrentTimeInMicroseconds();

   private:
    int m_elementsNr;
    inline static long long m_startTime = 0;
    ThreadSafeQueue<int>* m_queue;
    std::vector<NumberInfo>* m_storage;
    std::atomic_bool* m_completed;
    inline static std::atomic<size_t> m_order = 1;
};