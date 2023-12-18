#include <iostream>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

using namespace std;

void rotate(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();

    vector<vector<int>>matrix_t(n, vector<int>(m, 0));
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < m; col++) {
            int ori_col = row;
            int ori_row = m - col - 1;
            matrix_t[row][col] = matrix[ori_row][ori_col];
        }
    }
    matrix = matrix_t;
}
void rotate1(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < (n + 1) / 2; ++j) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[n - j - 1][i];
            matrix[n - j - 1][i] = matrix[n - i - 1][n - j - 1];
            matrix[n - i - 1][n - j - 1] = matrix[j][n - i - 1];
            matrix[j][n - i - 1] = temp;
        }
    }
}

TEST(mat, rotate_mat) {
    vector<vector<int>> matrix = {{1, 2, 3},
                                  {4, 5, 6},
                                  {7, 8, 9}};
    //rotate(matrix);
    rotate1(matrix);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

}
