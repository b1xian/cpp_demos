//
// Created by bigaoxian on 2021/10/19.
//
#include <vector>

#include <gtest/gtest.h>

template <class ContainerT>
class Foo {
//    typename ContainerT::iterator _it;
    decltype(ContainerT().begin()) _it;
public:
    void func(ContainerT& container) {
        _it = container.begin();
    }
};

TEST(testCase, test_decltype_1) {
    typedef const std::vector<int> container_t;
    container_t arr;
    Foo<container_t> foo;
    foo.func(arr);
}