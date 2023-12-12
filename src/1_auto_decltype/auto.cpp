//
// Created by bigaoxian on 2021/10/19.
//

#include <gtest/gtest.h>

class Foo {
public:
    static int get() {
        return 0;
    }
};

class Bar {
public:
    static const char* get() {
        return "0";
    }
};

// auto和后置返回类型
template <class A>
auto func() -> decltype(A::get()) {
    auto val = A::get();
    return val;
}

TEST(testCase, test_auto_1){
    EXPECT_EQ(func<Foo>(), 0);
    EXPECT_EQ(func<Bar>(), "0");
}