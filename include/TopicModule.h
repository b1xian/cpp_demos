//
// Created by bigaoxian on 2021/10/25.
//

#ifndef CPP11_DEMO_TOPICMODULE_H
#define CPP11_DEMO_TOPICMODULE_H



#include "build_config.h"
typedef std::conditional<IS_COMPRESSED_IMAGE_TYPE, int, float>::type traits_A;

class TopicModule {
public:
    const bool isCompressedImageType = IS_COMPRESSED_IMAGE_TYPE;

    traits_A a;
};

#endif //CPP11_DEMO_TOPICMODULE_H
