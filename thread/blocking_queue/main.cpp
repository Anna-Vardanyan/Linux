#include <iostream>
#include "blocking_queue.h"

int main(){
    LimitedBlockingQueue<int> queue(5);

    std::thread producer([&](){
        for(int i = 0; i < 5; ++i){
            queue.m_push(i);
            std::cout << "Producer: " << i << '\n';
        }
    });

    std::thread consumer([&](){
        for(int i = 0; i < 5; ++i){
            int tmp = queue.m_front();
            queue.m_pop();
            std::cout << "consumer: " << tmp << '\n';
        }
    });

    producer.join();
    consumer.join();
    return 0;
}