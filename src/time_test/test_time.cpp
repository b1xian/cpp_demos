//
// Created by sheldon on 2021/12/3.
//

#include <vector>
#include <iostream>

int main () {

    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto iter = v1.begin();
    for (; iter != v1.end(); ++iter) {
        if (*iter == 5) break;
    }

    std::vector<int> v2(v1.begin(), iter);
    std::vector<int> v3(iter, v1.end());

    int k = 10;
    for (int i = 0; i < v3.size(); ++i) {
        v3[i] = k++;
    }
    v2.insert(v2.end(), v3.begin(), v3.end());

    for (int i = 0; i < v2.size(); ++i) {
        printf("%d\n", v2[i]);
    }
    //printf("~~~~~~~~~~~~~~~~~~\n");
    //
    //for (int i = 0; i < v2.size(); ++i) {
    //    printf("%d\n", v2[i]);
    //}

    return 0;
}

