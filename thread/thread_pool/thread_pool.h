#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <tuple>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>

class ThreadPool {
public:
    explicit ThreadPool(std::mutex& m, std::condition_variable& cv,
                        std::size_t threads = std::thread::hardware_concurrency())
                        : stop(false)
                        , queue_mutex(m)
                        , condition(cv)
                        {
        for(size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return stop.test() || !this->tasks.empty();
                        });
                        if (stop.test() && this->tasks.empty()) {
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool(){
        stop.test_and_set();
        condition.notify_all();
        for(auto& worker: workers){
            worker.detach();
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
                [func=std::forward<F>(f), args_tuple=std::forward_as_tuple(std::forward<Args>(args)...)]() mutable {
                    return std::apply(std::forward<F>(func), std::forward<std::tuple<Args...>>(args_tuple));
                }
        );

        auto res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex& queue_mutex;
    std::condition_variable& condition;
    std::atomic_flag stop;
};

#endif //THREAD_POOL_H