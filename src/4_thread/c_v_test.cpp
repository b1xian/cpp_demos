//
// Created by bigaoxian on 2021/10/26.
//

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>

template<typename T>
class SyncQueue {
private:
    bool is_full() const {
        return m_queue.size() == m_max_size;
    }

    bool is_empty() const {
        return m_queue.empty();
    }

public:
    SyncQueue(int max_size) : m_max_size(max_size) {}

    void put(const T& x) {
        std::lock_guard<std::mutex> locker(m_mutex);
//        while(is_full()) {
//            m_not_full.wait(m_mutex);
//        }
        // 更优雅的写法
        m_not_full.wait(locker, [this](){
            return !is_full();
        });
        m_queue.push_back(x);
        m_not_empty.notify_one();
    }

    void take(const T& x) {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this]{
            return is_empty();
        });
//        while(is_empty()) {
//            m_not_empty.wait(m_mutex);
//        }
        x = m_queue.front();
        m_queue.pop_front();
        m_not_full.notify_one();
    }

private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable_any m_not_empty;
    std::condition_variable_any m_not_full;
    int m_max_size;
};
