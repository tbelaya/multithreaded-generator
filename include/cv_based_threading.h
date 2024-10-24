#pragma once

#include "consumer.h"

void initializeDistribution(int to);
void initializeStartTime();

void produce(ThreadSafeQueue<int>& queue, int elements, std::atomic_bool& completed);
void consume(ThreadSafeQueue<int>& queue,
             std::vector<NumberInfo>& storage,
             int elements,
             std::atomic_bool& completed);