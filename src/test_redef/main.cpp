#include "interface.h"

// 临时重命名int64来避免冲突
#define int64 serialport_int64
#include "SerialPort_global.h"
#undef int64


#include <vector>
#include <iostream>
#include <algorithm>



int main() {

  int64_t opencv_value = 0;          // 从 Opencv.h 定义
  serialport_int64 serial_value = 0; // 从 CSerialPort.h 定义


  std::vector<int> v1 = {1, 3, 5};
  v1.insert(v1.begin(), 0);
  std::string str{" "};
  for (auto v : v1) {
    str.append(std::to_string(v)).append(",");
  }
  printf("v1: %s\n", str.c_str());

  v1 = {1, 3, 5};
  v1.insert(v1.begin() + 1, 2);
  str = {" "};
  for (auto v : v1) {
    str.append(std::to_string(v)).append(",");
  }
  printf("v1: %s\n", str.c_str());

  v1 = {1, 3, 5};
  v1.insert(v1.begin() + 2, 4);
  str = {" "};
  for (auto v : v1) {
    str.append(std::to_string(v)).append(",");
  }
  printf("v1: %s\n", str.c_str());

  v1 = {1, 3, 5};
  v1.insert(v1.begin() + 3, 6);
  str = {" "};
  for (auto v : v1) {
    str.append(std::to_string(v)).append(",");
  }
  printf("v1: %s\n", str.c_str());


  printf("front:%d, back:%d \n", v1.front(), v1.back());
  printf("index_0:%d, index_%d:%d \n", v1[0], v1.size() - 1, v1[v1.size() - 1]);

  int latest_time = v1.back();
  auto expired_iter = std::find_if(v1.begin(), v1.end(),
                                   [&] (int time) {
                                     return (latest_time - time) <= 3;
                                   });
  if (expired_iter != v1.end()) {
    v1.erase(v1.begin(), expired_iter);
  }

  str = {" "};
  for (auto v : v1) {
    str.append(std::to_string(v)).append(",");
  }
  printf("v1: %s\n", str.c_str());
  return 0;
}