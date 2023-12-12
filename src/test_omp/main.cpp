//omp.cc
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
//#pragma omp parallel for
//    for (char i = 'a'; i <= 'z'; i++)
//        std::cout << i << std::endl;

//#ifndef _OPENMP
//    fprintf(stderr, "OpenMP not supported\n");
//#else
//    fprintf(stderr, "OpenMP supported~~\n");
//#endif


    omp_set_num_threads(4);
//#pragma omp parallel
//    {
//        cout << "Hello" << ", I am Thread " << omp_get_thread_num() << endl;
//    }

#pragma omp parallel for
    for (int i = 0; i < 30; i++)
        printf("i = %d, I am Thread %d\n", i, omp_get_thread_num());

    return 0;
}