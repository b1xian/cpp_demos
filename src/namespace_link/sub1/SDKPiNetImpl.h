//
// Created by sheldon on 2022/3/4.
//

#ifndef CPP11_DEMO_SDKPINETIMPL_H
#define CPP11_DEMO_SDKPINETIMPL_H

namespace TKDL {

class SDKPiNetImpl {
public:
    static SDKPiNetImpl* getInst();

private:
    static SDKPiNetImpl* __inst;
    SDKPiNetImpl() = default;

};

}


#endif //CPP11_DEMO_SDKPINETIMPL_H
