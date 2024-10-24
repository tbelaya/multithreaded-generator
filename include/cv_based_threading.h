#pragma once

#include "consumer.h"

/**
 * @file cv_based_threading.h
 * @brief A header file defining functions for a producer-consumer model
 *        using condition variable (CV) for synchronization.
 *
 * This file contains function declarations for initializing the random
 * number distribution and start time, as well as functions for
 * producing and consuming integers in a thread-safe manner using
 * a thread-safe queue.
 */

/**
 * @brief Initializes the random number distribution.
 *
 * This function sets up the uniform distribution for generating
 * random numbers within the specified range.
 *
 * @param to The upper limit for the random number generation.
 */
void initializeDistribution(int to);

/**
 * @brief Initializes the start time for tracking generation.
 *
 * This function records the start time for the generation process.
 */
void initializeStartTime();

/**
 * @brief Produces integers and adds them to the thread-safe queue.
 *
 * This function generates a specified number of random integers and
 * pushes them into the provided thread-safe queue. It also signals
 * when the production is completed.
 *
 * @param queue Reference to the thread-safe queue for storing
 *              produced integers.
 * @param elements The number of integers to produce.
 * @param completed Reference to an atomic boolean that indicates
 *                  whether production is complete.
 */
void produce(ThreadSafeQueue<int>& queue, int elements, std::atomic_bool& completed);

/**
 * @brief Consumes integers from the thread-safe queue and stores their
 *        information.
 *
 * This function retrieves integers from the provided thread-safe queue
 * and stores information about each consumed integer in the specified
 * storage vector.
 *
 * @param queue Reference to the thread-safe queue from which integers
 *              will be consumed.
 * @param storage Reference to a vector of NumberInfo to store
 *                information about consumed numbers.
 * @param elements The number of elements to consume.
 * @param completed Reference to an atomic boolean that indicates
 *                  whether consumption is complete.
 */
void consume(ThreadSafeQueue<int>& queue,
             std::vector<NumberInfo>& storage,
             int elements,
             std::atomic_bool& completed);