#include <iostream>

void print_arr1(int arr[][3]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_arr2(int (*arr)[3]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_arr3(int* arr, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", *(arr + m * i + j));
        }
        printf("\n");
    }
}

void print_arr4(int** arr, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

/**
 * 指针数组（array of pointers）：即用于存储指针的数组，也就是数组元素都是指针，如 int* a[4]，表示数组 a 中的元素都为 int 型指针；
 * 数组指针（a pointer to an array）：即指向数组的指针，如 int (*a)[4]，表示指向数组 a 的指针。
 * @return
 */
int main() {
    int arr_2dim[2][3] = {{0, 1, 2}, {3,4,5}};
    // arr是一个数组的指针
    int (*arr)[3] = arr_2dim;

    printf("print_arr1~~~~~~~~~~~~~~\n");
    print_arr1(arr);
    //print_arr4(arr, 2, 3);

    printf("print_arr2~~~~~~~~~~~~~~\n");
    print_arr2(arr);

    printf("print_arr3~~~~~~~~~~~~~~\n");
    print_arr3((int*)arr, 2, 3);

    printf("print_arr3~~~~~~~~~~~~~~\n");
    int *arr_heap = new int[2 * 3]{0, 1, 2, 3, 4, 5};
    print_arr3(arr_heap, 2, 3);


    printf("print_arr4~~~~~~~~~~~~~~\n");
    int k = 0;
    // 创建一个数组，长度为2，每个元素都是int*
    int **arr_2dim_heap = new int* [2];
    for(int i = 0; i < 2; i++) {
        arr_2dim_heap[i] = new int[3];
        for (int j = 0; j < 3; j++) {
            arr_2dim_heap[i][j] = k++;
        }
    }
    print_arr4(arr_2dim_heap, 2, 3);
}