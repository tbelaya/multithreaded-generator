#pragma once
#include <mutex>
#include <queue>

namespace core
{

/**
 * @class ThreadSafeQueue
 * @brief A thread-safe queue implementation with a fixed size.
 *
 * This class provides a thread-safe wrapper around a standard queue,
 * ensuring that push and pop operations are safe to call from multiple
 * threads. The queue has a maximum size, and pushing elements when full
 * will fail as well as popping elements when empty.
 *
 * @tparam T The type of elements stored in the queue.
 */
template <typename T>
class ThreadSafeQueue
{
   public:
    /**
     * @brief Constructor for a ThreadSafeQueue with a fixed size.
     *
     * @param size The maximum size of the queue.
     */
    explicit ThreadSafeQueue(size_t size) : m_size(size) {}

    /**
     * @brief Attempts to push a value to the queue.
     *
     * This method is thread-safe. It will only add the value if the
     * queue has not reached its maximum size.
     *
     * @param val The value to be added to the queue.
     * @return true if the value was successfully added, false if the queue is full.
     */
    [[nodiscard]] bool tryPush(const T& val)
    {
        std::scoped_lock lock(m_mtx);
        if (m_queue.size() >= m_size)
        {
            return false;
        }
        m_queue.push(val);
        return true;
    }

    /**
     * @brief Attempts to pop a value from the queue.
     *
     * This method is thread-safe. It will only remove the front value if the
     * queue is not empty.
     *
     * @param[out] val The value that was removed from the queue.
     * @return true if a value was successfully removed, false if the queue was empty.
     */
    [[nodiscard]] bool tryPop(T& val)
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
    size_t m_size;          ///< Maximum size of the queue.
    std::queue<T> m_queue;  ///< The underlying standard queue.
    std::mutex m_mtx;       ///< Mutex to ensure thread-safe access.
};

}  // namespace core