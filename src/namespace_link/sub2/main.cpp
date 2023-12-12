//
// Created by sheldon on 2022/3/4.
//

#include "../sub1/SDKPiNet.h"

using namespace TKDL;

int main() {
    void *data_ptr = 0;
    char *cfg_path = 0;
    SDK_PINET_INIT(data_ptr, cfg_path);
    SDK_PINET_INFER(data_ptr);
    SDK_PINET_VERSION();


    return 0;

}