#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

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
    float A[10][2] = {
            10, 30.4,
            11, 31.3,
            12, 32.3,
            13, 33.2,
            14, 34.3,
            15, 35.6,
            16, 36.5,
            17, 37.6,
            18, 38.4,
            19, 39.3,
    };

    const int stateNum=4;
    // 观测量，移动距离
    const int measureNum=2;

    KalmanFilter KF(stateNum, measureNum, 0);
    KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
                                                 1, 0, 1, 0,
                                                 0, 1, 0, 1,
                                                 0, 0, 1, 0,
                                                 0, 0, 0, 1);
    setIdentity(KF.measurementMatrix); // 测量矩阵H
    setIdentity(KF.processNoiseCov, Scalar::all(1e-1)); // 系统噪声方差矩阵Q
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1)); // 测量噪声方差矩阵R
    setIdentity(KF.errorCovPost, Scalar::all(1));
    Mat measurement = Mat::zeros(measureNum, 1, CV_32F);


    std::cout << KF.measurementNoiseCov << std::endl;
    std::cout << KF.measurementMatrix << std::endl;
    // 初始状态值
    KF.statePost.at<float>(0, 0) = A[0][0];
    KF.statePost.at<float>(1, 0) = A[0][1];
    cout << "init state0=" << A[0][0] << ", " << A[0][1] << endl;

    int i= 1;
    for (i = 1; i < 9; i++) {
      printf("~~~~~~~~~~~~~~~~round %d~~~~~~~~~~~~~~~~~\n", i);
        Mat prediction = KF.predict(); // 预测单位delta为1s
        printf("pre %.2f, y:%.2f %.2f, %.2f\n", prediction.at<float>(0), prediction.at<float>(1)
                , prediction.at<float>(2), prediction.at<float>(3));
//        if (i == 1) {
//          KF.statePost.at<float>(2, 0) = 1.0;
//          KF.statePost.at<float>(3, 0) = 1.0;
//        }
        measurement.at<float>(0) = (float)A[i][0];
        measurement.at<float>(1) = (float)A[i][1];
        printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
        KF.correct(measurement);
        printf("cor %.2f, y:%.2f vx:%.2f vy:%.2f\n",
                KF.statePost.at<float>(0), KF.statePost.at<float>(1),
                KF.statePost.at<float>(2), KF.statePost.at<float>(3));
    }

    printf("~~~~~~~~~~~~~~~~round 9~~~~~~~~~~~~~~~~~%d\n", i);
    // 预测小车运动0.5s的位置
    KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
                                                 1, 0, 0.5, 0,
                                                 0, 1, 0,   0.5,
                                                 0, 0, 1,   0,
                                                 0, 0, 0,   1);
    Mat prediction = KF.predict();
    printf("pre %.2f, y:%.2f\n", prediction.at<float>(0), prediction.at<float>(1));
    measurement.at<float>(0) = (float)A[i][0];
    measurement.at<float>(1) = (float)A[i][1];
    printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
    KF.correct(measurement);
    printf("cor %.2f, y:%.2f vx:%.2f vy:%.2f\n",
           KF.statePost.at<float>(0), KF.statePost.at<float>(1),
           KF.statePost.at<float>(2), KF.statePost.at<float>(3));

//    printf("trans %.2f, y:%.2f\n", KF.transitionMatrix.at<float>(2), KF.transitionMatrix.at<float>(7));
//    std::cout << KF.statePost << std::endl;


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
