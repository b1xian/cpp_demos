#include <iostream>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

using namespace std;

int Palindrome (string s, int left, int right) {
    while (left >= 0 && right < s.size()) {
        if (s[left] == s[right]) {
            left--;
            right++;
        } else {break;}
    }
    int start = left + 1;
    return right - start;
}
// 中心扩散
string longestPalindrome(string s) {
    size_t len = s.size();
    if (len < 2) {
        return s;
    }
    int max_len = 1;
    int begin = 0;
    for (size_t i = 0; i < len - 1; ++i) {
        // 奇数回文字符串判断
        int odd_len = Palindrome(s, i, i);
        // 偶数回文字符串判断
        int even_len = Palindrome(s, i, i + 1);
        int curr_len = max(odd_len, even_len);
        if (curr_len > max_len) {
            max_len = curr_len;
            begin = i - (max_len - 1) / 2;
        }
    }
    std::string substr = s.substr(begin, max_len);
    return substr;
}

string dt_solve (string s) {

    size_t len = s.size();
    if (len < 2) {
        return s;
    }
    int max_len = 1;
    int begin = 0;
    // 初始化g规划矩阵
    vector<vector<int>> dp(len, vector<int>(len, 0));
    for (int i = 0; i < len; ++i) {
        // 对角线置为true，每个长度为1的子串都是回文的
        dp[i][i] = 1;
    }
    for (int j = 1; j < len; ++j) {
        for (int i = 0; i < j; i++) {
            if (s[i] != s[j]) {
                // 字符不同，确定不是回文
                dp[i][j] = false;
            } else {
                // 字符相同，要看子串是不是回文
                if (j - i < 3) {
                    dp[i][j] = true;
                } else {
                    // 取决于子串是不是回文
                    dp[i][j] = dp[i + 1][j - 1];
                }
            }
            if (dp[i][j]) {
                if (j - i + 1 > max_len) {
                    max_len = j - i + 1;
                    begin = i;
                }
            }
        }
    }
    return s.substr(begin, max_len);
}

TEST(Is_Palindrome, is_Palindrome) {
    //string s = "bc";
    //size_t len = 0;
    //size_t start = 0;
    //Palindrome(s, 0, 0, len, start);
    //printf("start = %ld, len = %ld\n", start, len);
    //printf("substr:%s\n", s.substr(start, len).c_str());


    string s = "cbc";
    string substr = longestPalindrome(s);
    std::cout << substr << std::endl;
}
