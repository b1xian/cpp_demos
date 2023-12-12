//
// Created by sheldon on 2021/11/9.
//
#include "visitor.h"
#include "visitor.hpp"

int main() {
    ConcretVisitor v;
    std::unique_ptr<Element> ele1(new ConcretEle1);
    std::unique_ptr<Element> ele2(new ConcretEle2);

    ele1->accept(v);
    ele2->accept(v);


    return 0;
}
