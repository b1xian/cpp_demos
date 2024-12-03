//
// Created by bigaoxian
//

#include <iostream>
#include <Eigen/Dense>


const double PI = 3.1415926536;
const double ARC_TO_DEG = 180.0 / PI;
const double DEG_TO_ARC = PI / 180.0;




int main() {


  double born_time_s = 1732933494.6737769;
  printf("born_time_s:%f \n", born_time_s);

  // 精度溢出
//  float born_time = float(born_time_s);
//  float born_time = static_cast<float>(born_time_s);
//  printf("float born_time_s:%f \n", born_time);

  long long int_part = static_cast<long long>(born_time_s);
  double frac_part = born_time_s - int_part;
  printf("int_part:%ld \n", int_part);
  printf("frac_part:%f \n", frac_part);
  float born_time = static_cast<float>(int_part);
  printf("float born_time_s:%f \n", born_time);

  return 0;
}