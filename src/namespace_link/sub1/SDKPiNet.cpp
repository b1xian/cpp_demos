//
// Created by sheldon on 2022/3/4.
//

#include "SDKPiNet.h"
#include "SDKPiNetImpl.h"

#include <iostream>


namespace TKDL {

//using namespace TKDL;
int SDK_PINET_INIT(void* const data_ptr, const char* const cfg_path) {
    auto instance = SDKPiNetImpl::getInst();
    std::cout << "SDK_PINET_INIT~" <<std::endl;
}

int SDK_PINET_INFER(void* const data_ptr) {
    auto instance = SDKPiNetImpl::getInst();
    std::cout << "SDK_PINET_INFER~" <<std::endl;
}

void SDK_PINET_VERSION() {
    std::cout << "SDK_PINET_VERSION~" <<std::endl;
}

}