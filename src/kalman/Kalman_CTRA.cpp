#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Dense>

using namespace cv;
using namespace std;

cv::Mat_<float> GetTransMat(float dt) {
  double a = 0.5 * dt * dt;
  return  (cv::Mat_<float>(6, 6) <<
                        1, 0, dt, 0, a, 0,
                        0, 1, 0, dt, 0, a,
                        0, 0, 1, 0, dt, 0,
                        0, 0, 0, 1, 0,  dt,
                        0, 0, 0, 0, 1,  0,
                        0, 0, 0, 0, 0,  1);
}


int main() {
  std::vector<std::vector<float>> steps;
  steps.push_back({0.1, 1, 30.4});
  steps.push_back({0.2, 1, 31.3});
  steps.push_back({0.3, 1, 32.3});
  steps.push_back({0.4, 1, 33.2});
  steps.push_back({0.5, 1, 34.3});
  steps.push_back({0.6, 1, 35.5});
  steps.push_back({0.7, 1, 36.5});
  steps.push_back({0.8, 1, 37.6});
  steps.push_back({0.9, 1, 38.4});
  steps.push_back({1.0, 1, 39.5});
  steps.push_back({0.95, 1, 38.95});

  const int stateNum=6;
  // 观测量，移动距离
  const int measureNum=2;
  float dt = 0.1;

  KalmanFilter KF(stateNum, measureNum, 0);
  KF.transitionMatrix = GetTransMat(dt);
  setIdentity(KF.measurementMatrix); // 测量矩阵H
  setIdentity(KF.processNoiseCov, Scalar::all(1e-1)); // 系统噪声方差矩阵Q
  std::cout << KF.processNoiseCov << std::endl; // Q
  setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1)); // 测量噪声方差矩阵R
  std::cout << KF.measurementNoiseCov << std::endl; //R
  setIdentity(KF.errorCovPost, Scalar::all(1));
  std::cout << KF.errorCovPost << std::endl; //P
  Mat measurement = Mat::zeros(measureNum, 1, CV_32F);
//    std::cout << KF.measurementNoiseCov << std::endl;
//    std::cout << KF.measurementMatrix << std::endl;

  std::vector<float> init_step = steps[0];
  // 初始状态值
  KF.statePost.at<float>(0, 0) = init_step[1];
  KF.statePost.at<float>(1, 0) = init_step[2];
  KF.statePost.at<float>(2, 0) = 0.0;
  KF.statePost.at<float>(3, 0) = 10.0;
  cout << "init state0= " << KF.statePost << endl;

  cv::Mat X_backup;
  cv::Mat P_backup;
  float framestamp_backup;
  float last_stamp = init_step[0];
  int i= 1;
  for (; i <= 9; i++) {
    std::vector<float> curr_step = steps[i];
    dt = curr_step[0] - last_stamp;
    last_stamp = curr_step[0];
    printf("~~~~~~~~~~~~~~~~step %d~~~~~~~~~~~~~~~~~\n", i);
    Mat prediction = KF.predict(); // 预测单位delta为1s
    printf("pred %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
           prediction.at<float>(0), prediction.at<float>(1),
           prediction.at<float>(2), prediction.at<float>(3),
            prediction.at<float>(4), prediction.at<float>(5));
    KF.transitionMatrix = GetTransMat(dt);
    measurement.at<float>(0) = curr_step[1];
    measurement.at<float>(1) = curr_step[2];
    printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
    KF.correct(measurement);
    printf("corr %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
            KF.statePost.at<float>(0), KF.statePost.at<float>(1),
            KF.statePost.at<float>(2), KF.statePost.at<float>(3),
            KF.statePost.at<float>(4), KF.statePost.at<float>(5));
    if (i == 8) {
      // 保存状态
      printf("step 8, backup state:%f %f %f\n", curr_step[0], curr_step[1], curr_step[2]);
      X_backup = KF.statePost.clone();
      P_backup = KF.errorCovPost.clone();
      framestamp_backup = last_stamp;
    }
  }


  printf("~~~~~~~~~~~~~~~~step 8.5~~~~~~~~~~~~~~~~~\n");
  std::vector<float> step_85 = steps[10];
  dt = step_85[0] - framestamp_backup;
  last_stamp = step_85[0];
  KF.statePost = X_backup;
  KF.errorCovPost = P_backup;
  KF.transitionMatrix = GetTransMat(dt);
  Mat prediction = KF.predict();
  printf("pred %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
         prediction.at<float>(0), prediction.at<float>(1),
         prediction.at<float>(2), prediction.at<float>(3),
         prediction.at<float>(4), prediction.at<float>(5));
  // update
  measurement.at<float>(0) = step_85[1];
  measurement.at<float>(1) = step_85[2];
  printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
  KF.correct(measurement);
  printf("corr %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
         KF.statePost.at<float>(0), KF.statePost.at<float>(1),
         KF.statePost.at<float>(2), KF.statePost.at<float>(3),
         KF.statePost.at<float>(4), KF.statePost.at<float>(5));

  printf("~~~~~~~~~~~~~~~~re update step 9~~~~~~~~~~~~~~~~~\n");
  std::vector<float> step_9 = steps[9];
  dt = step_9[0] - last_stamp;
  last_stamp = step_9[0];
  KF.transitionMatrix = GetTransMat(dt);
  prediction = KF.predict();
  printf("pred %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
         prediction.at<float>(0), prediction.at<float>(1),
         prediction.at<float>(2), prediction.at<float>(3),
         prediction.at<float>(4), prediction.at<float>(5));
  // update
  measurement.at<float>(0) = step_9[1];
  measurement.at<float>(1) = step_9[2];
  printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
  KF.correct(measurement);
  printf("corr %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
         KF.statePost.at<float>(0), KF.statePost.at<float>(1),
         KF.statePost.at<float>(2), KF.statePost.at<float>(3),
         KF.statePost.at<float>(4), KF.statePost.at<float>(5));



  return 0;
}
