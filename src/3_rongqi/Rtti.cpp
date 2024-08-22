#include <iostream>
#include <typeinfo>
#include <memory>

class A {
 public:
  virtual ~A()=default; // 基类需要一个虚析构函数，以启用RTTI
  virtual void print() {
    printf("I am A\n");
  }
};

class B : public A {
 public:
  void print() override {
    std::cout << "I am B" << std::endl;
  }
};

class C : public A {
 public:
  void print() override {
    std::cout << "I am C" << std::endl;
  }
};

//void identify(A* obj) {
//  if (B* b = dynamic_cast<B*>(obj)) {
//    std::cout << "Object is of type B" << std::endl;
//    b->print();
//  } else if (C* c = dynamic_cast<C*>(obj)) {
//    std::cout << "Object is of type C" << std::endl;
//    c->print();
//  } else {
//    std::cout << "Object is of unknown type" << std::endl;
//  }
//}

int main() {
  std::shared_ptr<A> Ab;
  std::shared_ptr<A> Ac;
  Ab.reset(new B());
  Ac.reset(new C());


  if (std::dynamic_pointer_cast<B>(Ab)) {
    printf("Ab is b\n");
    Ab->print();
  }
  if (!std::dynamic_pointer_cast<C>(Ab)) {
    printf("Ab is not C\n\n");
  }

  if (std::dynamic_pointer_cast<C>(Ac)) {
    printf("Ac is C\n");
    Ac->print();
  }
  if (!std::dynamic_pointer_cast<B>(Ac)) {
    printf("Ac is not B\n");
  }
  return 0;
}
