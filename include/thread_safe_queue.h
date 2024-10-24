#pragma once
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
   public:
    ThreadSafeQueue(size_t size) : m_size(size) {}

    bool tryPush(const T& val)
    {
        std::scoped_lock lock(m_mtx);
        if (m_queue.size() >= m_size)
        {
            return false;
        }
        m_queue.push(val);
        return true;
    }
    bool tryPop(T& val)
    {
        std::scoped_lock lock(m_mtx);
        if (m_queue.size() == 0)
        {
            return false;
        }
        val = m_queue.front();
        m_queue.pop();
        return true;
    }

   private:
    size_t m_size;
    std::queue<T> m_queue;
    std::mutex m_mtx;
};