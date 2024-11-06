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

    while (!m_completed->load())
    {
        if (m_queue->tryPop(randValue))
        {
            int index = randValue - 1;
            size_t expected = 0;
            // Check if the generated number is already present in the storage. Do it in a
            // thread-safe manner using the atomic operation compare_exchange_strong.
            if ((*m_storage)[index].m_order.compare_exchange_strong(expected, m_order))
            {
                // Calculate time it took to generate the value
                auto endTime = getCurrentTimeInMicroseconds();
                auto timeTaken = endTime - m_startTime;

                // Save the generated number
                (*m_storage)[index].m_order = m_order++;
                (*m_storage)[index].m_generationTime = timeTaken;

                std::cout << std::format(
                    "number = {:05}, order = {:05}, generation_time = {:010}\n", randValue,
                    (*m_storage)[index].m_order.load(), timeTaken);

                if (m_order == m_elementsNr + 1)
                {
                    // All the numbers are generated
                    m_completed->store(true);
                }
                m_startTime = getCurrentTimeInMicroseconds();
            }
        }
    }
    std::cout << "Consumer finished task.\n";
}