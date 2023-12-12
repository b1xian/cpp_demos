#include <functional>
#include <iostream>
#include <vector>

void callback(int val, std::vector<int>* vec) {
    vec->push_back(val);
    if(vec->size() > 30) {
        vec->pop_back();
    }
}


int main () {
    std::vector<int> v1;
    std::vector<int> v2;

    auto callback_1 = std::bind(&callback, std::placeholders::_1, &v1);
    auto callback_2 = std::bind(&callback, std::placeholders::_1, &v2);

    callback_1(1);
    callback_1(1);
    callback_1(1);

    std::cout << "v1 size :" << v1.size() << std::endl;
    for (auto i : v1) {
        std::cout << i << std::endl;
    }


    callback_2(2);
    callback_2(2);
    std::cout << "v2 size :" << v2.size() << std::endl;
    for (auto i : v2) {
        std::cout << i << std::endl;
    }
    return 0;
}
