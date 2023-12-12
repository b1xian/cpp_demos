#ifndef CPP11_DEMO_SYNCQUEUE_HPP
#define CPP11_DEMO_SYNCQUEUE_HPP

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

template<typename T>
class SyncQueue {
public:
    SyncQueue(int max_size) :m_max_size(max_size), m_need_stop(false) {}
    void put(const T& x){
        Add(x);
    }
    void put(T&& x){
        Add(std::forward<T>(x));
    }
    void take(std::list<T>& list) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this]{
            return m_need_stop || not_empty();
        });
        if (m_need_stop) return;
        list = std::move(m_queue);
        m_not_full.notify_one();
    }
    void take(T& t) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this]{
            return m_need_stop || not_empty();
        });
        if (m_need_stop) return;
        t = m_queue.front();
        m_queue.pop_front();
        m_not_full.notify_one();
    }
    void stop () {
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_need_stop = true;
        }
        m_not_full.notify_all();
        m_not_empty.notify_all();
    }
    bool empty() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }
    bool full() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == m_max_size;
    }
    size_t size() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }
    int count() {
        return m_queue.size();
    }
private:
    bool not_full() const {
        bool is_full = m_queue.size() >= m_max_size;
        if (is_full) cout << "缓冲区满了，等待。。。" << endl;
        return !is_full;
    }
    bool not_empty() const {
        bool is_empty = m_queue.empty();
        if (is_empty) cout << "缓冲区空了，需要等待。。。异步层的线程ID:" << this_thread::get_id << endl;
        return !is_empty;
    }
    template<typename F>
    void Add(F&& f) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_full.wait(locker, [this] {
            return m_need_stop || not_full();
        });
        if (m_need_stop) return;
        m_queue.push_back(std::forward<F>(f));
        m_not_empty.notify_one();
    }

private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
    int m_max_size;
    bool m_need_stop;
};

#endif //CPP11_DEMO_SYNCQUEUE_HPP
