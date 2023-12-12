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

int main() {

    printf("~~~~~~test eigen~~~\n");
    //x:  0.532611
    //y: -0.511136
    //z:  0.471955
    //w: -0.481999

    float q_x = -0.007773;
    float q_y = -0.807750;
    float q_z = 0.589328;
    float q_w = 0.013093;
    printf("q: %f %f %f %f\n", q_w, q_x, q_y, q_z);

    Eigen::Quaterniond quaternion(q_w, q_x, q_y, q_z);
    Eigen::Vector3d eulerAngle = quaternion.matrix().eulerAngles(2, 1, 0);

    float x_arc = eulerAngle[2]; // roll
    float y_arc = eulerAngle[1]; // pitch
    float z_arc = eulerAngle[0]; // yaw
    printf("arc: %f %f %f \n", x_arc, y_arc, z_arc);


    float x_deg = x_arc * ARC_TO_DEG; // roll
    float y_deg = y_arc * ARC_TO_DEG; // pitch
    float z_deg = z_arc * ARC_TO_DEG; // yaw
    printf("deg: \nroll = %f\npitch = %f\nyaw = %f \n", x_deg, y_deg, z_deg);

    //std::cout << quaternion.matrix() << std::endl;

    //float roll  = 82.838;
    //float pitch = -178.281;
    //float yaw   = 93.2086 - 3;
    //
    //quaternion =  Eigen::AngleAxisd(yaw * DEG_TO_ARC,   Eigen::Vector3d::UnitZ()) *
    //              Eigen::AngleAxisd(pitch * DEG_TO_ARC, Eigen::Vector3d::UnitY()) *
    //              Eigen::AngleAxisd(roll * DEG_TO_ARC,  Eigen::Vector3d::UnitX());
    //printf("q: \n%f \n%f \n%f \n%f\n", quaternion.w(),
    //                           quaternion.x(),
    //                           quaternion.y(),
    //                           quaternion.z());


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


    return 0;
}