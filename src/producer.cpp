#include "producer.h"

#include <iostream>
#include <thread>

void Producer::produce()
{
    while (!m_completed->load())
    {
        if (!m_queue->tryPush(m_distribution(m_generator)))
        {
            // If the queue is full and the push operation fails, yield the current thread
            // to allow other threads to run.
            std::this_thread::yield();
        }
    }
    std::cout << "Producer finished task.\n";
}