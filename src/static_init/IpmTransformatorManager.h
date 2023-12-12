//
// Created by sheldon on 2022/2/18.
//

#ifndef CPP11_DEMO_IPMMANAGER_H
#define CPP11_DEMO_IPMMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>


using IpmTransformatorMap = std::unordered_map<std::string, int>;
class IpmTransformatorManager {
public:
    static int InitIpmTransformator();
    static int GetIpmTransformator(const std::string& camera_node);
private:
    static IpmTransformatorMap imp_transformator_map;
};


#endif //CPP11_DEMO_IPMMANAGER_H
