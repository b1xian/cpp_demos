//
// Created by sheldon on 2021/11/9.
//
#include "observer_1.h"
#include "observer_2.hpp"


struct stA {
    int a, b;
    void print(int a, int b) {
        cout << "stA: " << a << "," << b << endl;
    }
};

void print(int a, int b) {
    cout << "print func: " << a << " ," << b << endl;
}


int main () {
    Events<std::function<void(int, int)>> MyEvent;
    int key = MyEvent.Connect(print);
    printf("key %d\n", key);

    stA t;
    int  lambad_key = MyEvent.Connect([&t](int a, int b) {t.a = a, t.b = b; });

//    std::function<void(int,int)> f = std::bind(&stA::print, &t, std::placeholders::_1, std::placeholders::_2);
//    MyEvent.Connect(f);


    int a = 1, b = 2;
    MyEvent.Notify(a, b);

    printf("t.a :%d, t.b : %d \n ", t.a, t.b);
    return 0;
}