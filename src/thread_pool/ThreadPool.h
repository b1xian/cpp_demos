//
// Created by sheldon on 2021/11/9.
//

#ifndef CPP11_DEMO_THREADPOOL_H
#define CPP11_DEMO_THREADPOOL_H

#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>

#include "SyncQueue.hpp"

const int MaxTaskCount = 100;

class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(int num_threads = std::thread::hardware_concurrency()) : m_queue(MaxTaskCount) {
        start(num_threads);
    }
    ~ThreadPool() {
        stop();
    }
    void stop() {
        std::call_once(m_flag, [this]{
            stop_thread_group();
        });
    }
    void add_task(Task&& task) {
        m_queue.put(std::forward<Task>(task));
    }
    void add_task(const Task& task) {
        m_queue.put(task);
    }
private:
    void start(int num_threads) {
        m_running = true;
        // 初始化线程组
        for (int i = 0; i < num_threads; ++i) {
            m_thread_group.push_back(std::make_shared<std::thread>(&ThreadPool::run_in_thread, this));
        }
    }
    void run_in_thread() {
        while (m_running) {
            // 获取任务分别执行
            std::list<Task> list;
            m_queue.take(list);
            for (auto& task : list) {
                if (!m_running) return;
                task();
            }
        }
    }
    void stop_thread_group() {
        m_queue.stop();
        for (auto thread : m_thread_group) {
            if (thread) thread->join();
        }
        m_thread_group.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_thread_group;
    SyncQueue<Task> m_queue;
    atomic_bool m_running;
    std::once_flag m_flag;
};

#endif //CPP11_DEMO_THREADPOOL_H
