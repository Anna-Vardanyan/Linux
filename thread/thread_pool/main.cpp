#include <iostream>
#include <thread>
#include <mutex>
#include "thread_pool.h"

int main(){
    std::mutex m;
    std::condition_variable c;
    ThreadPool pool(m,c,4);

    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " executed by thread " << std::this_thread::get_id() << std::endl;
        });
    }

    int a = 1;
    auto fut = pool.enqueue([](int a, double d){ return a + d;}, a, 2.4);
    std::cout << fut.get() << "\n";

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}