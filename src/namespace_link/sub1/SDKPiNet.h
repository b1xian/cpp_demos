//
// Created by sheldon on 2022/3/4.
//

#ifndef CPP11_DEMO_SDKPINET_H
#define CPP11_DEMO_SDKPINET_H

namespace TKDL {

int SDK_PINET_INIT(void* const data_ptr, const char* const cfg_path);

int SDK_PINET_INFER(void* const data_ptr);

void SDK_PINET_VERSION();

}


#endif //CPP11_DEMO_SDKPINET_H
