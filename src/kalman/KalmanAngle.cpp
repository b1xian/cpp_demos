#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

const float PI = 3.14159265358979323846;

// 将角度转换为正弦和余弦表示
void angleToSinCos(float angle, float &sin_val, float &cos_val) {
  sin_val = std::sin(angle);
  cos_val = std::cos(angle);
}

// 将正弦和余弦表示转换回角度
float sinCosToAngle(float sin_val, float cos_val) {
  return std::atan2(sin_val, cos_val);
}

int main() {
  const int stateNum = 4;   // [sin(heading), cos(heading), delta_sin, delta_cos]
  const int measureNum = 2; // [sin(heading), cos(heading)]

  KalmanFilter KF(stateNum, measureNum, 0);

  // 状态转移矩阵，假设状态是：sin(heading), cos(heading), delta_sin, delta_cos
  KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
                                               1, 0, 1, 0,
                                                0, 1, 0, 1,
                                                0, 0, 1, 0,
                                                0, 0, 0, 1);
  setIdentity(KF.measurementMatrix); // 测量矩阵H
  setIdentity(KF.processNoiseCov, Scalar::all(1e-2)); // 系统噪声方差矩阵Q
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1)); // 测量噪声方差矩阵R
  setIdentity(KF.errorCovPost, Scalar::all(1));
  Mat measurement = Mat::zeros(measureNum, 1, CV_32F);

  // 初始状态值
  float initial_heading = 0.0;
  float initial_sin, initial_cos;
  angleToSinCos(initial_heading, initial_sin, initial_cos);
  KF.statePost.at<float>(0) = initial_sin; // sin(heading)
  KF.statePost.at<float>(1) = initial_cos; // cos(heading)
//  KF.statePost.at<float>(2) = 0;           // delta_sin
//  KF.statePost.at<float>(3) = 0;           // delta_cos
  cout << "Initial state: " << KF.statePost << endl;

  // 模拟数据
  std::vector<float> headings;
  for (int i = 0; i < 31; i ++) {
    headings.push_back(i * 0.1);
  }
  headings.push_back(M_PI);
  for (int i = 31; i >= 0; i--) {
    headings.push_back(-i * 0.1);
  }
  for (int i = 0; i < 31; i ++) {
    headings.push_back(i * 0.1);
  }

  for (int i = 0; i < headings.size(); ++i) {
//    printf("~~~~~~measure:%.4f\n", headings[i]);
//    continue;
    // 将角度转换为正弦和余弦表示
    float sin_heading, cos_heading;
    angleToSinCos(headings[i], sin_heading, cos_heading);

    measurement.at<float>(0) = sin_heading;
    measurement.at<float>(1) = cos_heading;

    // 卡尔曼滤波
    Mat prediction = KF.predict();
    KF.correct(measurement);

    // 将滤波后的状态转换回角度
    float filtered_heading = sinCosToAngle(KF.statePost.at<float>(0), KF.statePost.at<float>(1));

    cout << "============Round ============ " << i + 1 << ":" << endl;
//    cout << "Prediction: " << prediction << endl;
//    cout << "Measurement: " << measurement << endl;
//    cout << "Corrected state (sin, cos): " << KF.statePost.at<float>(0) << ", " << KF.statePost.at<float>(1) << endl;
    printf("~~~~~~measure:%.4f, corr:%.4f\n", headings[i], filtered_heading);


    std::vector<double> k_data = {1.,1.,1.,
                                  1.,1.,1.,
                                  1.,1.,1.};
    cv:Mat K = cv::Mat(3, 3, CV_64F, k_data.data());
    std::cout << K <<std::endl;

  }

  return 0;
}
