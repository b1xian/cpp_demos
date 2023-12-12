//
// Created by sheldon on 2021/11/9.
//

#include "SyncQueue.hpp"
#include "ThreadPool.h"

int main() {
    ThreadPool pool(2);

    std::thread t1([&pool]{
       for (int i = 0; i < 100; i++) {
           auto t_id = this_thread::get_id();
           pool.add_task([t_id]{
               cout << "同步层线程1的线程ID：" << t_id << endl;
           });
       }
    });

    std::thread t2([&pool]{
        for (int i = 0; i < 100; i++) {
            auto t_id = this_thread::get_id();
            pool.add_task([t_id]{
                cout << "同步层线程2的线程ID：" << t_id << endl;
            });
        }
    });

    this_thread::sleep_for(std::chrono::seconds(2));

    getchar();
    pool.stop();
    t1.join();
    t2.join();
    return 0;
}