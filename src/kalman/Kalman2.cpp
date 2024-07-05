#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Eigen/Dense>

using namespace cv;
using namespace std;

struct Object {
  int id;
  int age;
};

double Rect2DIou(const cv::RotatedRect& rect1, const cv::RotatedRect& rect2) {
  float area_a = rect1.size.area();
  float area_b = rect2.size.area();

  std::vector<cv::Point2f> inter_points;
  cv::rotatedRectangleIntersection(rect1, rect2, inter_points);
  float inter_area = 0.;
  if (!inter_points.empty()) {
    std::vector<cv::Point2f> order_inter_points(inter_points.size());
    cv::convexHull(inter_points, order_inter_points);
    inter_area = cv::contourArea(order_inter_points);
  }
  float union_area = area_a + area_b - inter_area;
  return inter_area / (union_area + 1e-8);
}

float Rect2DIou(const cv::Rect_<float>& rect1, const cv::Rect_<float>& rect2) {
  float in = (rect1 & rect2).area();
  float un = (rect1 | rect2).area();
  if (un < DBL_EPSILON) {
    return 0;
  } else {
    return in / un;
  }
}

int main() {
    float A[30][2] = {
            {1, 30.4},
            {1, 31.3},
            {1, 32.3},
            {1, 33.2},
            {1, 34.3},
            {1, 35.5},
            {1, 36.5},
            {1, 37.6},
            {1, 38.4},
            {1, 39.3},
            {1, 40.5},
            {1, 41.7},
            {1, 42.9},
            {1, 44.2},
            {1, 45.6},
            {1, 47.0},
            {1, 48.8},
            {1, 50.8},
            {1, 54.0},
            {1, 56.5},
            {1, 54.0},
    };

    const int stateNum=6;
    // 观测量，移动距离
    const int measureNum=2;

    double dt = 0.1;
    double a = 0.5 * dt * dt;
//    A = np.array([[1,0,t,0,0.5*t*t,0],
//                  [0,1,0,t,0,0.5*t*t],
//                  [0,0,1,0,t,0],
//                  [0,0,0,1,0,t],
//                  [0,0,0,0,1,0],
//                  [0,0,0,0,0,1]])

  KalmanFilter KF(stateNum, measureNum, 0);
    KF.transitionMatrix = (cv::Mat_<float>(6, 6) <<
                                                 1, 0, dt, 0, a, 0,
                                                 0, 1, 0, dt, 0, a,
                                                 0, 0, 1, 0, dt, 0,
                                                 0, 0, 0, 1, 0,  dt,
                                                 0, 0, 0, 0, 1,  0,
                                                 0, 0, 0, 0, 0,  1);
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
    // 初始状态值
    KF.statePost.at<float>(0, 0) = A[0][0];
    KF.statePost.at<float>(1, 0) = A[0][1];
    KF.statePost.at<float>(2, 0) = 0.0;
    KF.statePost.at<float>(3, 0) = 10.0;
    cout << "init state0= " << A[0][0] << ", " << A[0][1] << endl;

    int i= 1;
    for (; i < 20; i++) {
        printf("~~~~~~~~~~~~~~~~round %d~~~~~~~~~~~~~~~~~\n", i);
        Mat prediction = KF.predict(); // 预测单位delta为1s
        printf("pred %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
               prediction.at<float>(0), prediction.at<float>(1),
               prediction.at<float>(2), prediction.at<float>(3),
                prediction.at<float>(4), prediction.at<float>(5));
        measurement.at<float>(0) = (float)A[i][0];
        measurement.at<float>(1) = (float)A[i][1];
        printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
        KF.correct(measurement);
        printf("corr %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
             KF.statePost.at<float>(0), KF.statePost.at<float>(1),
                KF.statePost.at<float>(2), KF.statePost.at<float>(3),
                KF.statePost.at<float>(4), KF.statePost.at<float>(5));
    }

  printf("~~~~~~~~~~~~~~~~round %d~~~~~~~~~~~~~~~~~\n", i);
//    // 预测小车运动0.5s的位置
  KF.transitionMatrix = (cv::Mat_<float>(6, 6) <<
                                               1, 0, dt, 0, a, 0,
                                                0, 1, 0, dt, 0, a,
                                                0, 0, 1, 0, dt, 0,
                                                0, 0, 0, 1, 0,  dt,
                                                0, 0, 0, 0, 1,  0,
                                                0, 0, 0, 0, 0,  1);

  std::cout << KF.transitionMatrix << std::endl;
    Mat prediction = KF.predict();
    printf("pred %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
           prediction.at<float>(0), prediction.at<float>(1),
           prediction.at<float>(2), prediction.at<float>(3),
           prediction.at<float>(4), prediction.at<float>(5));
    measurement.at<float>(0) = (float)A[i][0];
    measurement.at<float>(1) = (float)A[i][1];
    printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
    KF.correct(measurement);
    printf("corr %.2f,%.2f velo(%.2f,%.2f), acc(%.2f,%.2f)\n",
           KF.statePost.at<float>(0), KF.statePost.at<float>(1),
           KF.statePost.at<float>(2), KF.statePost.at<float>(3),
           KF.statePost.at<float>(4), KF.statePost.at<float>(5));



//  -- 5, sim:0.10, bev:0.00 0.37, trk:0.10,
//  r:(2.3,1.5)
//  loc x:(-4.3,-4.2) y:(53.2,50.8)
//  l:(4.8,4.8) w:(1.6,1.6) yaw:(1.5708,1.5708)

//  cv::RotatedRect det_rect_bev;
//  det_rect_bev.center = cv::Point2f(53.2, -4.3);
////  det_rect_bev.center = cv::Point2f(-4.3, 53.2 );
//  det_rect_bev.size = cv::Size2f(1.5 * 1.6, 4.8 * 2.3);
////  det_rect_bev.size = cv::Size2f(4.8 * 2.3, 1.5 * 1.6);
//  det_rect_bev.angle = 1.5708 * 180 / CV_PI;
//
//  cv::RotatedRect target_rect_bev;
//  target_rect_bev.center = cv::Point2f(51.8, -4.3);
////  target_rect_bev.center = cv::Point2f(-4.2, 50.8);
//  target_rect_bev.size = cv::Size2f(1.5 * 1.6, 4.8 * 2.3);
////  target_rect_bev.size = cv::Size2f(4.8 * 2.3, 1.5 * 1.6);
//  target_rect_bev.angle = 1.5708 * 180 / CV_PI;
//
//  double iou =  Rect2DIou(det_rect_bev, target_rect_bev);
//
//  std::cout << det_rect_bev.boundingRect2f() << std::endl;
//  std::cout << target_rect_bev.boundingRect2f() << std::endl;
//  double b_iou =  Rect2DIou(det_rect_bev.boundingRect2f(), target_rect_bev.boundingRect2f());


//  printf("IOU: %.3f %.3f\n", iou, b_iou);


  return 0;
}
