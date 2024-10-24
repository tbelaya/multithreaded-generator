#include <iostream>
#include <numeric>
#include <thread>

#include "consumer.h"
#include "cv_based_threading.h"
#include "producer.h"

enum
{
    QUEUE_SIZE_MAX = 1000
};

int main(int argc, char* argv[])
{
    bool cvMode = false;

    // Check command-line arguments
    std::vector<std::string> arguments(argv + 1, argv + argc);
    for (const auto& arg : arguments)
    {
        if (arg == "--cv" || arg == "-cv")
        {
            // Standard approach with condition variables
            cvMode = true;
        }
    }

    int elementsNr{};
    std::cout << "Please enter the number of elements to generate: ";
    std::cin >> elementsNr;

    if (elementsNr <= 0 || elementsNr > RAND_MAX)
    {
        std::cout << "Incorrect value. Must be positive integer not larger than RAND_MAX.";
        return -1;
    }

    // Create a shared thread-safe queue
    ThreadSafeQueue<int> queue(QUEUE_SIZE_MAX);
    // Create s storage for random numbers
    std::vector<NumberInfo> storage(elementsNr);
    // Completion flag
    std::atomic_bool complete(false);
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    if (!cvMode)
    {
        Consumer::setStartTime();
        startTime = std::chrono::high_resolution_clock::now();
        // Perform generation of random numbers asynchronously
        Producer producerOne(queue, elementsNr, complete);
        Producer producerTwo(queue, elementsNr, complete);
        Consumer consumerOne(queue, storage, elementsNr, complete);
        Consumer consumerTwo(queue, storage, elementsNr, complete);

        std::thread prodThreadOne(&Producer::produce, &producerOne);
        std::thread consThreadOne(&Consumer::consume, &consumerOne);
        std::thread prodThreadTwo(&Producer::produce, &producerTwo);
        std::thread consThreadTwo(&Consumer::consume, &consumerTwo);

        // Wait for all threads to finish
        prodThreadOne.join();
        consThreadOne.join();
        prodThreadTwo.join();
        consThreadTwo.join();
    }
    else
    {
        initializeDistribution(elementsNr);
        initializeStartTime();

        startTime = std::chrono::high_resolution_clock::now();
        // Perform generation of random numbers asynchronously
        std::thread prodThreadOne([&]() { produce(queue, elementsNr, complete); });
        std::thread consThreadOne([&]() { consume(queue, storage, elementsNr, complete); });
        std::thread prodThreadTwo([&]() { produce(queue, elementsNr, complete); });
        std::thread consThreadTwo([&]() { consume(queue, storage, elementsNr, complete); });

        // Wait for all threads to finish
        prodThreadOne.join();
        consThreadOne.join();
        prodThreadTwo.join();
        consThreadTwo.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalWorkTime =
        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    auto totalTime =
        std::accumulate(storage.begin(), storage.end(), 0, [](int sum, const NumberInfo& element)
                        { return sum + element.m_generationTime; });

    std::cout << "Generation completed. Total generation time: " << totalTime << " microseconds."
              << std::endl;
    std::cout << "Total execution time: " << totalWorkTime << " microseconds." << std::endl;
    return 0;
}