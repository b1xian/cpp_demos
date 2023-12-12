//
// Created by bigaoxian on 2021/10/26.
//

#include <future>
#include <thread>

#include <gtest/gtest.h>

int func (int x) {
    return x + 2;
}

TEST(testCase, test_feature_2) {
    std::packaged_task<int(int)> tsk(func);
    std::future<int> fut = tsk.get_future();

    std::thread(std::move(tsk), 2).detach();
    int value = fut.get();
    EXPECT_EQ(value, 4);


}

//TEST(testCase, test_feature_1){
//    std::packaged_task<int()> task([](){
//        return 7;
//    });
//    std::thread t1(std::ref(task));
//    std::future<int> f1 = task.get_future();
//    auto r1 = f1.get();
//    EXPECT_EQ(r1, 7);
//}

TEST(testCase, test_feature_async){
    std::future<int> f1 = std::async(std::launch::async, []{
        return 8;
    });
    EXPECT_EQ(f1.get(), 8);
}