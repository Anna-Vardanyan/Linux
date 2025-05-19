#ifndef BLOCKING_QUEUE_BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_BLOCKING_QUEUE_H

#include <queue>
#include <condition_variable>
#include <random>
#include <mutex>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

template <typename T>
class LimitedBlockingQueue{

    std::size_t m_capacity;
    std::queue<T> m_requests;
    std::mutex m_mutex;
    std::condition_variable m_requestsState;

public:
    explicit LimitedBlockingQueue(std::size_t capacity) : m_capacity(capacity){};

    T m_front() {
        std::unique_lock<std::mutex> l{m_mutex};
        m_requestsState.wait(l, [this](){return !m_requests.empty();});
        return m_requests.front();
    }

    void m_push(const T& elem) {
        std::unique_lock<std::mutex> l{m_mutex};
        m_requestsState.wait(l, [this](){return m_requests.size() < m_capacity;});
        m_requests.push(elem);
        l.unlock();
        m_requestsState.notify_all();
    }

    void m_pop() {
        std::unique_lock<std::mutex> l{m_mutex};
        m_requestsState.wait(l, [this](){return !m_requests.empty();});
        m_requests.pop();
        l.unlock();
        m_requestsState.notify_all();
    }
};

#endif //BLOCKING_QUEUE_BLOCKING_QUEUE_H