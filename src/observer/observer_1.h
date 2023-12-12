//
// Created by sheldon on 2021/11/7.
//
#ifndef CPP11_DEMO_OBSERVER_1_H
#define CPP11_DEMO_OBSERVER_1_H
#include <list>

using namespace std;

class Subject;
// 观察者接口类
class Observer {
public:
    ~Observer();
    void Update(Subject *sub) {};

protected:
    Observer();
};
// 主题类
class Subject {
public:
    virtual ~Subject();
    virtual void Attach(Observer*);
    virtual void Detach(Observer*);
    virtual void Notify();

protected:
    Subject();

private:
    list<Observer*> _observers;
};

#endif //CPP11_DEMO_OBSERVER_1_H

