#include "consumer.h"

#include <chrono>
#include <format>
#include <iostream>

void Consumer::setStartTime()
{
    m_startTime = getCurrentTimeInMicroseconds();
}

long long Consumer::getCurrentTimeInMicroseconds()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

void Consumer::consume()
{
    int randValue{};

    while (!m_completed)
    {
        if (m_queue->tryPop(randValue))
        {
            int index = randValue - 1;
            size_t expected = 0;
            if (!m_completed &&
                (*m_storage)[index].m_order.compare_exchange_strong(expected, m_order))
            {
                // Calculate time it took to generate the value
                auto endTime = getCurrentTimeInMicroseconds();
                auto timeTaken = endTime - m_startTime;

                (*m_storage)[index].m_order = m_order++;
                (*m_storage)[index].m_generationTime = timeTaken;

                std::string output =
                    std::format("number = {:05}, order = {:05}, generation_time = {:010}\n",
                                randValue, (*m_storage)[index].m_order.load(), timeTaken);
                std::cout << output;

                if (m_order == m_elementsNr + 1)
                {
                    m_completed->store(true);
                }
                m_startTime = getCurrentTimeInMicroseconds();
            }
        }
    }
    std::cout << "Consumer finished task.\n";
}