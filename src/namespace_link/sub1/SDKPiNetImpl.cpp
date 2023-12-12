//
// Created by sheldon on 2022/3/4.
//

#include "SDKPiNetImpl.h"

#include <iostream>

namespace TKDL {

SDKPiNetImpl* SDKPiNetImpl::__inst = nullptr;

SDKPiNetImpl* SDKPiNetImpl::getInst() {
    if (__inst == nullptr) {
        __inst =  new SDKPiNetImpl();
    }

    return __inst;
}



}
