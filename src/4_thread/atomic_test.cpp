//
// Created by bigaoxian on 2021/10/26.
//

#include <atomic>

struct AtomicCounter {
    std::atomic<int> counter;
    void increment() {
        ++counter;
    }
    void decrement() {
        --counter;
    }
    int get() {
        return counter.load();
    }
};

