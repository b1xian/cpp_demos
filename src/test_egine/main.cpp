//
// Created by sheldon on 23-5-11.
//

#include <iostream>
#include <Eigen/Dense>

#include <string>
#include <chrono>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <sstream>

const double PI = 3.1415926536;
const double ARC_TO_DEG = 180.0 / PI;
const double DEG_TO_ARC = PI / 180.0;

//static std::string GetDateStr(const std::string& date_fmt, uint64_t time_stamp) {
//    strftime(buf, 64, date_fmt.c_str(), time_stamp);
//
//    std::string date_str(buf);
//    return date_str;
//}

std::string GetTimeStampDateStr(const char* date_fmt, uint64_t timestamp) {
    std::chrono::time_point<std::chrono::system_clock> tp{std::chrono::nanoseconds(timestamp)};
    // 转换为time_t
    time_t now_time_t = std::chrono::system_clock::to_time_t(tp);

    struct tm now_tm;
    localtime_r(&now_time_t, &now_tm);

    // 格式化为字符串
    char buf[64]= {0};
    std::strftime(buf, 64, date_fmt, &now_tm);
    std::string date_str(buf);
    date_str += std::to_string(timestamp / 1000000 % 1000);

    return date_str;
}

std::string GetCurrTimeStampDateStr(const char* date_fmt) {
    auto now = std::chrono::system_clock::now();    // 转换为time_t
    time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    struct tm now_tm;
    localtime_r(&now_time_t, &now_tm);

    // 格式化为字符串
    char buf[64]= {0};
    std::strftime(buf, 64, date_fmt, &now_tm);
    std::string date_str(buf);
    date_str += std::to_string(now.time_since_epoch().count() / 1000000 % 1000);

    return date_str;
}
double GetHeading(double heading) {
  return heading - M_PI / 2;
}

enum RotationUnitType { ANGLE, RADIAN };
Eigen::Quaterniond Radian2Quaternion(double roll, double pitch, double yaw,
                                                 RotationUnitType rot_unit) {
  if (rot_unit == RotationUnitType::ANGLE) {
    roll *= DEG_TO_ARC;
    pitch *= DEG_TO_ARC;
    yaw *= DEG_TO_ARC;
  }
  Eigen::Quaterniond quaternion =
          Eigen::AngleAxisd(yaw,   Eigen::Vector3d::UnitZ()) *
          Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
          Eigen::AngleAxisd(roll,  Eigen::Vector3d::UnitX());
  return quaternion;
}

Eigen::Matrix3d eulerToRotationMatrix(double roll, double pitch, double yaw,
                                      RotationUnitType rot_unit) {
  if (rot_unit == RotationUnitType::ANGLE) {
    roll *= DEG_TO_ARC;
    pitch *= DEG_TO_ARC;
    yaw *= DEG_TO_ARC;
  }

  // 创建绕X轴的旋转矩阵
  Eigen::Matrix3d R_x;
  R_x = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

  // 创建绕Y轴的旋转矩阵
  Eigen::Matrix3d R_y;
  R_y = Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY());

  // 创建绕Z轴的旋转矩阵
  Eigen::Matrix3d R_z;
  R_z = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());

  // 组合旋转矩阵，根据ZYX的旋转顺序
  Eigen::Matrix3d R = R_z * R_y * R_x;

  return R;
}
Eigen::Affine3d GetAffine(const Eigen::Quaterniond& orientation,
                                  const Eigen::Vector3d& xyz) {
  Eigen::Matrix4d centerm;
  centerm.block<3, 3>(0, 0) = orientation.toRotationMatrix();
  centerm.block<3, 1>(0, 3) = xyz;
  centerm.block<1, 4>(3, 0) << 0, 0, 0, 1;
  return Eigen::Affine3d(centerm);
}
inline double EncodeHeadingRad(double heading) {
  return (heading >= 0 && heading <= M_PI) ?
         heading : heading + 2 * M_PI;
}

inline double DecodeHeadingRad(double heading) {
  return heading > M_PI ? heading - 2 * M_PI : heading;
}

double ego2rfu(double ego_rot) {
  ego_rot = -ego_rot;
  double rfu_rot = 0.;
  if (ego_rot >=  -M_PI && ego_rot <= M_PI_2) {
    rfu_rot = ego_rot + M_PI_2;
  }

  if (ego_rot > M_PI_2 && ego_rot < M_PI) {
    rfu_rot = ego_rot + -3 / M_PI_2;
  }
  return rfu_rot;
}

double CalcVelocityHeading(const Eigen::Vector3d& velocity) {
  double velo_heading = 0.0;
  if (fabs(velocity[0]) < 1e-4) {
    // 横向速度比较小的情况下
    velo_heading = velocity[1] >= 0.f ? 0 : M_PI;
  } else {
    velo_heading = std::atan(velocity[0] / velocity[1]);
    if (velo_heading > 0 && velocity[1] < 0) {
      velo_heading -= M_PI;
    } else if (velo_heading < 0  && velocity[1] < 0) {
      velo_heading += M_PI;
    }
  }
  return velo_heading;
//  return std::atan(velocity[0] / velocity[1]);
}

double EgoYaw2Rfu(double ego_yaw) {
  ego_yaw = -ego_yaw;
  double rfu_yaw = 0.;
  if (ego_yaw >= -M_PI && ego_yaw <= M_PI_2) {
    rfu_yaw = ego_yaw + M_PI_2;
  }

  if (ego_yaw > M_PI_2 && ego_yaw < M_PI) {
    rfu_yaw = ego_yaw + (-3.0 / 2.0) * M_PI;
  }
  return rfu_yaw;
}

double PointEuclideanDistance(double x1, double y1, double x2, double y2) {
  return  std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}



int main() {
/**right 200
 * roll = 0.532446
pitch = -124.276764
yaw = 88.157425



 */

/**
 * front r120
 *roll = 89.058922
pitch = -60.325195
yaw = 0.382056
 *
 * back r 120
 * roll = 80.992958
pitch = -120.134460
yaw = 8.016597
 */
  printf("~~~~~~test eigen~~~\n");
//  float q_w = -0.6151885580574651;
//  float q_x = -0.6075161610864784;
//  float q_y = 0.356172035726809;
//  float q_z = -0.3544130824619007;
  float q_w = -0.33286707486457795;
  float q_x = -0.6165592094899691;
  float q_y = 0.6335914075414883;
  float q_z = -0.3280490511399165;

  printf("q: %f %f %f %f\n", q_w, q_x, q_y, q_z);

    Eigen::Quaterniond quaternion(q_w, q_x, q_y, q_z);

    Eigen::Vector3d eulerAngle = quaternion.matrix().eulerAngles(2, 1, 0);
    auto mat = quaternion.matrix();

    float x_arc = eulerAngle[2]; // roll
    float y_arc = eulerAngle[1]; // pitch
    float z_arc = eulerAngle[0]; // yaw
    printf("arc: %f %f %f \n", x_arc, y_arc, z_arc);


    float x_deg = x_arc * ARC_TO_DEG; // roll
    float y_deg = y_arc * ARC_TO_DEG; // pitch
    float z_deg = z_arc * ARC_TO_DEG; // yaw
    printf("deg: \nroll = %f\npitch = %f\nyaw = %f \n", x_deg, y_deg, z_deg);


  Eigen::Affine3d global2sensor = GetAffine(quaternion, Eigen::Vector3d(0.0, 0.0, 0.0));
//  Eigen::Matrix4d global_pose;
//  global_pose.block<3, 3>(0, 0) = orientation.toRotationMatrix();
//  global_pose.block<3, 1>(0, 3) = global_xyz;
//  global_pose.block<1, 4>(3, 0) << 0, 0, 0, 1;
  Eigen::Quaterniond quaternion1(-0.33286707486457795,-0.6165592094899691,0.6335914075414883,-0.3280490511399165);
  //std::cout << quaternion.matrix() << std::endl;
  Eigen::Quaterniond(global2sensor.rotation());
//    float roll  = -89.88177976075468;
//    float pitch = -0.7192224805790984;
//    float yaw   = -90.39237784285308;
////    float roll  = x_deg;
////    float pitch = y_deg;
////    float yaw   = z_deg;
//    quaternion =  Eigen::AngleAxisd(yaw * DEG_TO_ARC,   Eigen::Vector3d::UnitZ()) *
//                  Eigen::AngleAxisd(pitch * DEG_TO_ARC, Eigen::Vector3d::UnitY()) *
//                  Eigen::AngleAxisd(roll * DEG_TO_ARC,  Eigen::Vector3d::UnitX());
//    printf("q: %f %f %f %f\n", quaternion.w(),
//                                       quaternion.x(),
//                                       quaternion.y(),
//                                       quaternion.z());
//  yaw = 0.05;
////  quaternion =  Eigen::AngleAxisd(yaw,   Eigen::Vector3d::UnitZ()) *
////                Eigen::AngleAxisd(0.0, Eigen::Vector3d::UnitY()) *
////                Eigen::AngleAxisd(0.0,  Eigen::Vector3d::UnitX());
//  quaternion = Radian2Quaternion(0, 0, yaw, RotationUnitType::RADIAN);
//  printf("q: %f %f %f %f\n", quaternion.w(),
//         quaternion.x(),
//         quaternion.y(),
//         quaternion.z());
//  eulerAngle = quaternion.matrix().eulerAngles(2, 1, 0);
//  z_arc = eulerAngle[0]; // yaw
//  printf("1 arcz : %f \n", z_arc);
//
//  quaternion = Eigen::Quaterniond(cos(yaw / 2), 0, 0, sin(yaw / 2));
//  printf("q: %f %f %f %f\n", quaternion.w(),
//         quaternion.x(),
//         quaternion.y(),
//         quaternion.z());
//  eulerAngle = quaternion.matrix().eulerAngles(2, 1, 0);
//  z_arc = eulerAngle[0]; // yaw
//  printf("2 arcz : %f \n", z_arc);
    //std::string date_str = GetCurrDateStr("%Y-%m-%d %H:%M:%S.%f");
    //std::cout << date_str << std::endl;


    //double ts =1684896328463877376;
    //auto  date_str = GetTimeStampDateStr("%Y-%m-%d %H:%M:%S.", ts);
    //std::cout << date_str << std::endl;
    //for (int i = 0; i < 100; ++i) {
    //
    //    date_str = GetCurrTimeStampDateStr("%Y-%m-%d %H:%M:%S.");
    //    std::cout << date_str << std::endl;
    //}

    ////Eigen::
    //// 29102
    //float ql_x =   0.000000;
    //float ql_y =   0.000000;
    //float ql_z =   0.892794;
    //float ql_w =   0.450465;
    //float l_x = -1537.760654;
    //float l_y =  3060.330883;
    //
    //Eigen::Quaterniond q_l(ql_w, ql_x, ql_y, ql_z);
    //// 29104
    //float qr_x = 0.000000;
    //float qr_y = 0.000000;
    //float qr_z = 0.892686;
    //float qr_w = 0.450680;
    //float r_x = -1538.201899;
    //float r_y =  3060.928531;
    //Eigen::Quaterniond q_r(qr_w, qr_x, qr_y, qr_z);
    //Eigen::Quaterniond q_in = q_l.slerp(0.5, q_r);
    //
    //std::cout << q_in.coeffs() << std::endl;
    //q_in.x();
    //double x_in = (r_x + l_x) * 0.5;
    //double y_in = (r_y + l_y) * 0.5;
    //printf("x:%f y:%f\n", x_in, y_in);

    //int id = 888;
    //int max_id_ = 255;
    //if (0 <= id <= max_id_) {
    //    printf("recycle id %d\n", id);
    //} else {
    //    printf("recycle id %d invalid!!! max id:%d\n", id, max_id_);
    //}
    //int num = 3;
    //
    //num = num % 2 == 0 ? num : num - 1;
    //
    //printf("%d \n", num);



    //uint64_t num1 = 600;
    //uint64_t num2 = 500;
    //
    //int64_t difference = (num1 >= num2) ? static_cast<int64_t>(num1 - num2) : static_cast<int64_t>(num2 - num1);
    //
    ////int64_t difference = std::abs(static_cast<int64_t>(num1) - static_cast<int64_t>(num2));
    //
    ////uint64_t difference = num2 - num1;
    //
    //std::cout << "The difference is: " << difference << std::endl;

//    Eigen::Vector3d vec(4.0, 9.0, 16.0);
//
//  // 输出结果
//    std::cout << "Original vector: " << vec << std::endl;
//    vec = vec.array().square();
//    std::cout << "Vector after square root: " << vec << std::endl;
//
//
//    double heading = 1.5708;
//  double a_heading = GetHeading(heading);
//  printf("before heading:%f, after heading:%f\n", heading, a_heading);
//  Eigen::Vector2d v_xy;
//  v_xy << 0.06, 13.38;
//  double velo = v_xy.norm();
//  printf("velo:%f %f\n", v_xy(0), v_xy(1));
//
//  Eigen::VectorXd ctra_state_(6);
//  ctra_state_.setZero();
//  ctra_state_(2) = heading;
//  ctra_state_(3) = velo;
////  ctra_state_ <<     -1.1-5.025,
////                     -52.3812,
////                      2.22045e-16,
////                      13.3755,
////                      0.001,
////                      0;
//  std::cout << ctra_state_ << std::endl;
//  Eigen::Vector2d v_xy_state(ctra_state_(3) * cos(ctra_state_(2)),
//                       ctra_state_(3) * sin(ctra_state_(2)));
//  printf("v_xy:%f %f\n", v_xy_state(0), v_xy_state(1));
  // 初始欧拉角（以弧度为单位，如果你以度为单位，需要进行转换）

  double roll = 0;
  double pitch = 0;
  double yaw = -1.0722423417832683; // 初始时yaw为0度

  // 将欧拉角转换为四元数（这里假设roll, pitch, yaw是以弧度为单位的）
  Eigen::Quaterniond q = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
                         Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
                         Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

  // 执行一个yaw的90度旋转（以弧度为单位）
  q = q * Eigen::AngleAxisd(-M_PI_2, Eigen::Vector3d::UnitZ()); // M_PI_2是90度的弧度值

  // 从四元数转换回欧拉角（这可能不是精确的，特别是当接近万向锁时）
  Eigen::Vector3d eulerAngles = q.toRotationMatrix().eulerAngles(2, 1, 0); // ZYX顺序，注意Eigen的默认顺序可能与你期望的不同

  // 如果你需要度作为单位，进行转换
  x_arc = eulerAngles[2]; // roll
  y_arc = eulerAngles[1]; // pitch
  z_arc = eulerAngles[0]; // yaw

  printf("%f %f %f\n", x_arc, y_arc, z_arc);

  Eigen::Quaterniond sensor_orientation;
  sensor_orientation.w() = 0.70710678118654757;
  sensor_orientation.x() = -0.0;
  sensor_orientation.y() = -0.0;
  sensor_orientation.z() = -0.70710678118654746;

  sensor_orientation.w() = 0.7070935881588816,
  sensor_orientation.x() = 0.004319442625842828,
  sensor_orientation.y() = 0.004319442625842829,
  sensor_orientation.z() = -0.7070935881588817,
  eulerAngles = sensor_orientation.toRotationMatrix().eulerAngles(0, 1, 2);
  x_arc = eulerAngles[0]; // roll
  y_arc = eulerAngles[1]; // pitch
  z_arc = eulerAngles[2]; // yaw
  printf("%f %f %f\n", x_arc, y_arc, z_arc);


  double heading_rad = -z_arc + M_PI / 2;
  printf("rfu_rot:%f ego_rot = %f\n", z_arc, heading_rad);

  double z_arc2 = -1.4481;
  double heading_rad2 = -z_arc2 + M_PI / 2;
  printf("rfu_rot2:%f ego_rot2 = %f\n", z_arc2, heading_rad2);


//  -2.367862 -> rfu:-0.773731, det_heading:-2.228737
  double ego_yaw = -2.36;
  double rfu_yaw = EgoYaw2Rfu(ego_yaw);
  printf("ego_yaw:%f rfu_yaw = %f\n", ego_yaw, rfu_yaw);
//  Eigen::Vector3d velo;
//
//  double vx = 0;
//  double vy = 3;
//  velo << vx, vy, 0;
//  double v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = 3;
//  vy = 3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = 3;
//  vy = 0;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = 3;
//  vy = -3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = 0;
//  vy = -3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = -3;
//  vy = -3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = -3;
//  vy = 0;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = -3;
//  vy = 3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);
//
//  vx = 0;
//  vy = 3;
//  velo << vx, vy, 0;
//  v_heading = CalcVelocityHeading(velo);
//  printf("vx:%f vy:%f vH:%f\n", vx, vy, v_heading);

//  double heading_encode = EncodeHeadingRad(z_arc);

// 15 velo before :(-2.9, 14.6), filter:(-2.9, 6.0)
// -----fusion obj 15, heading det:1.6869, corr:1.6801, direction:1

//  Eigen::Vector3d v1, v2;
//  v1 << -2.9, 14.6, 0;
//  v2 << -2.9, 0.0, 0;
//  float eu_dis = PointEuclideanDistance(
//          v1[0], v1[1],
//          v2[0], v2[1]);
//  std::cout << eu_dis << std::endl;
//
//  Eigen::MatrixXd P_ = Eigen::MatrixXd::Identity(4, 4);
//  P_ << 1,0,0,0,
//        0,2,0,0,
//        0,0,3,0,
//        0,0,0,4;
//  printf("%f %f %f %f\n", P_(0), P_(5),  P_(10), P_(15));
  sensor_orientation.w() = 0.7070935881588816;
  sensor_orientation.x() = 0.004319442625842828;
  sensor_orientation.y() = 0.004319442625842829;
  sensor_orientation.z() = -0.7070935881588817;
  eulerAngles = sensor_orientation.toRotationMatrix().eulerAngles(2, 1, 0);
  x_arc = eulerAngles[0]; // roll
  y_arc = eulerAngles[1]; // pitch
  z_arc = eulerAngles[2]; // yaw
  printf("%f %f %f\n", x_arc, y_arc, z_arc);
  printf("rfu_rot:%f \n", x_arc);

  sensor_orientation.w() = 0.6428235052927631;
  sensor_orientation.x() = 0.6365682039102064;
  sensor_orientation.y() = 0.2902881782925809;
  sensor_orientation.z() = -0.3119160726176728;
  eulerAngles = sensor_orientation.toRotationMatrix().eulerAngles(0, 1, 2);
  x_arc = eulerAngles[0]; // roll
  y_arc = eulerAngles[1]; // pitch
  z_arc = eulerAngles[2]; // yaw
  x_deg = x_arc * ARC_TO_DEG; // roll
  y_deg = y_arc * ARC_TO_DEG; // pitch
  z_deg = z_arc * ARC_TO_DEG; // yaw
  printf("eulerAngles %f %f %f\n", x_deg, y_deg, z_deg);
//  printf("rfu_rot:%f \n", x_arc);
//
//  double z_arc2 = -1.4481;
//  double heading_rad2 = -z_arc2 + M_PI / 2;
//  printf("rfu_rot2:%f ego_rot2 = %f\n", z_arc2, heading_rad2);

//  yaw = 0.0638;
//  quaternion = Radian2Quaternion(0, 0, yaw, RotationUnitType::RADIAN);
//
//  auto r = eulerToRotationMatrix(0, 0, yaw, RotationUnitType::RADIAN);
//  std::cout << r << std::endl;
//  std::cout << quaternion.toRotationMatrix() << std::endl;



  z_deg = -54.0;
  quaternion =
          Eigen::AngleAxisd(x_deg * DEG_TO_ARC,   Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(y_deg * DEG_TO_ARC, Eigen::Vector3d::UnitY()) *
          Eigen::AngleAxisd(z_deg * DEG_TO_ARC,  Eigen::Vector3d::UnitZ());

  printf("q: \n%f,\n %f,\n %f,\n %f\n", quaternion.w(),
         quaternion.x(),
         quaternion.y(),
         quaternion.z());
  return 0;
}