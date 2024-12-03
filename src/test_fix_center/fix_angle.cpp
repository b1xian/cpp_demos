//
  // Created by sheldon on 23-5-11.
//

#include <iostream>
#include <Eigen/Dense>

#include <string>
#include <vector>

const double PI = 3.1415926536;
const double ARC_TO_DEG = 180.0 / PI;
const double DEG_TO_ARC = PI / 180.0;

enum RotationUnitType { ANGLE, RADIAN };


double RfuYaw2Ego(double rfu_yaw) {
  rfu_yaw = -rfu_yaw;
  double ego_yaw = 0.;

  if (rfu_yaw >= -M_PI && rfu_yaw <= M_PI_2) {
    ego_yaw = rfu_yaw + M_PI_2;
  }

  if (rfu_yaw > M_PI_2 && rfu_yaw < M_PI) {
    ego_yaw = rfu_yaw + (-3.0 / 2.0) * M_PI;
  }
  return ego_yaw;
}

std::pair<float, float> OrientationSimilarity(double heading_radian_1, double heading_radian_2) {
    // 计算两个弧度值的差值
  float diff = fabs(heading_radian_1 - heading_radian_2);

  if (diff > M_PI) {
    diff = 2 * M_PI - diff;
  }

  return {diff, 1 - diff / M_PI};
}

Eigen::Matrix3d EulerToRotationMatrix(double roll, double pitch, double yaw,
                                                  RotationUnitType rot_unit) {
  if (rot_unit == RotationUnitType::ANGLE) {
    roll *= DEG_TO_ARC;
    pitch *= DEG_TO_ARC;
    yaw *= DEG_TO_ARC;
  }

  Eigen::Matrix3d R_x;
  R_x = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

  Eigen::Matrix3d R_y;
  R_y = Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY());

  Eigen::Matrix3d R_z;
  R_z = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());

  // zyx
  return R_z * R_y * R_x;
}



int main() {
  Eigen::Quaterniond fixed_q;

  Eigen::Matrix3d mat = fixed_q.toRotationMatrix();

  return 0;
}
