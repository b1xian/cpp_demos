#include <iostream>
#include <deque>

#include <gtest/gtest.h>

#include "include/TopicModule.h"

std::deque<int> history_matrix_;

bool GetTransMatrix(int from, int to, std::string& match) {
    int from_index = abs(from);
    int to_index = abs(to);
    printf("from:%d\n", from);
    printf("to:%d\n", to);

    if (from > 0 || from_index >= history_matrix_.size() ||
        to > 0  || to_index >= history_matrix_.size()) {
        std::cout << "index invalkid!" << std::endl;
        return false;
    }

    // front [0, 1, 2, ...] back
    int max_index = history_matrix_.size() - 1;
    from_index = max_index - from_index;
    to_index = max_index - to_index;
    auto from_matrix = history_matrix_[from_index];
    auto to_matrix = history_matrix_[to_index];
    match = std::to_string(from_matrix) + "->" + std::to_string(to_matrix);

    return true;
}

int main(int argc,char **argv) {
    std::cout << "Hello, World!" << std::endl;
    TopicModule *t =  new TopicModule();
    std::cout << t->isCompressedImageType << std::endl;
    std::cout << "A: " << std::is_same<int, traits_A>::value << std::endl;

    testing::InitGoogleTest(&argc, argv);



    std::deque<int> queue;
    queue.push_back(0);
    queue.push_back(1);
    queue.push_back(2);
    printf("0:%d\n", queue.at(0));
    printf("1:%d\n", queue.at(1));
    printf("2:%d\n", queue.at(2));
    printf("back:%d\n", queue.back());
    printf("front:%d\n", queue.front());

    for (int i = 0; i < 10; ++i) {
        history_matrix_.push_back(i);
    }

    std::string match_res;
    int from = -1;
    int to = 0;
    bool res = GetTransMatrix(from, to, match_res);
    // 8->9
    printf("from, to matched:%s\n", match_res.c_str());
    //return RUN_ALL_TESTS();
}
