//
// Created by sheldon on 2021/11/8.
//

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <gtest/gtest.h>

using namespace std;

class NoCopyable {
protected:
    NoCopyable() = default;
    ~NoCopyable() = default;
    NoCopyable(const NoCopyable&) = delete; // 禁用拷贝构造
    NoCopyable& operator= (const NoCopyable&) = delete; // 禁用拷贝赋值
};

template <typename Func>
class Events : NoCopyable {
public:
    Events() {}
    ~Events() {}
    // 注册观察者
    int Connect(Func&& f) {
        return Assgin(f);
    }
    int Connect(const Func& f) {
        return Assgin(f);
    }
    // 移除观察者
    void Disconnect(int key) {
        m_connections.erase(key);
    }
    template<typename... Args>
    void Notify(Args&&... args) {
        for (auto& it : m_connections) {
            it.second(std::forward<Args>(args)...);
        }
    }
private:
    template<typename F>
    int Assgin(F&& f) {
        int k = m_observerId++;
        m_connections.emplace(k, std::forward<F>(f));
        return k;
    }
    int m_observerId = 0; // 观察者编号
    std::map<int, Func> m_connections; // 观察者列表
};



