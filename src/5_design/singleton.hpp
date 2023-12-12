//
// Created by sheldon on 2021/11/7.
//

#include <iostream>
#include <string>
#include <utility>

#include <gtest/gtest.h>

using namespace std;

template<typename T>
class Singleton {
public:
    template<typename... Args>
    static T* Instance(Args&&... args) {
        if (m_pInstance == nullptr) {
            m_pInstance = new T(std::forward<Args>(args)...); // 完美转发
        }
        return m_pInstance;
    }

    static T* getInstance() {
        return m_pInstance;
    }

    static void DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
private:
    Singleton(void);
    virtual ~Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
private:
    static T* m_pInstance;
};

struct A {
    A(const string&) {
        cout << "A() lvalue" << endl;
    }
    A(const string&&) {
        cout << "A() rvalue" << endl;
    }
};

struct B {
    B(const string&) {
        cout << "B() lvalue" << endl;
    }
    B(const string&&) {
        cout << "B() rvalue" << endl;
    }
};

struct C {
    C(int x, double y) {}
    void fun() {
        cout << "C fun() test" <<endl;
    }
};


TEST(testCase, test_singleton) {
    string str("bb");
    Singleton<A>::Instance(str);
    Singleton<B>::Instance(std::move(str));
    Singleton<C>::Instance(1, 3.14);
    Singleton<C>::getInstance()->fun();

    Singleton<A>::DestroyInstance();
    Singleton<B>::DestroyInstance();
    Singleton<C>::DestroyInstance();
    EXPECT_EQ(0, 0);
}
