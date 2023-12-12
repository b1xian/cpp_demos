//
// Created by sheldon on 2021/11/9.
//

#ifndef CPP11_DEMO_VISITOR_H
#define CPP11_DEMO_VISITOR_H

#include <iostream>
#include <memory>

using namespace std;

struct ConcretEle1;
struct ConcretEle2;

// 访问者基类
struct Visitor {
    virtual ~Visitor() {}
    virtual void Visit(ConcretEle1* ele) = 0;
    virtual void Visit(ConcretEle2* ele) = 0;
};

// 被访问者基类
struct Element {
    virtual ~Element() {}
    virtual void accept(Visitor& visitor) = 0;
};

// 具体的访问者
struct ConcretVisitor : public Visitor {
    void Visit(ConcretEle1* ele) {
        cout << "visit ConcretEle1" << endl;
    }
    void Visit(ConcretEle2* ele) {
        cout << "visit ConcretEle2" << endl;
    }
    ~ConcretVisitor() {}
};
// 具体的被访问者1
struct ConcretEle1 : public Element {
    void accept (Visitor& visitor) {
        visitor.Visit(this);
    }
};
// 具体的被访问者2
struct ConcretEle2 : public Element {
    void accept (Visitor& visitor) {
        visitor.Visit(this);
    }
};

#endif //CPP11_DEMO_VISITOR_H
