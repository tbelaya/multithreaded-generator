#include "producer.h"

#include <chrono>
#include <iostream>
#include <thread>

void Producer::produce()
{
    while (!m_completed->load())
    {
        if (!m_queue->tryPush(m_distribution(m_generator)))
        {
            std::this_thread::yield();
            // Alternatively
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    std::cout << "Producer finished task.\n";
}