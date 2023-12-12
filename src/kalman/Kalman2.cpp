#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

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

    // 初始状态值
    KF.statePost.at<float>(0, 0) = A[0][0];
    KF.statePost.at<float>(1, 0) = A[0][1];
    cout << "state0=" << KF.statePost << endl;

    for (int i = 1; i <= 9; i++) {
        Mat prediction = KF.predict(); // 预测单位delta为1s
        measurement.at<float>(0) = (float)A[i][0];
        measurement.at<float>(1) = (float)A[i][1];
        KF.correct(measurement);
        printf("~~~~~~~~~~~~~~~~round %d~~~~~~~~~~~~~~~~~\n", i);
        printf("pre %.2f, y:%.2f\n", prediction.at<float>(0), prediction.at<float>(1));
        printf("mea %.2f, y:%.2f\n", measurement.at<float>(0), measurement.at<float>(1));
        printf("cor %.2f, y:%.2f\n", KF.statePost.at<float>(0), KF.statePost.at<float>(1));
    }

    printf("~~~~~~~~~~~~~~~~round 10~~~~~~~~~~~~~~~~~\n");
    // 预测小车运动0.5s的位置
    KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<
                                                 1, 0, 0.5, 0,
                                                 0, 1, 0, 0.5,
                                                 0, 0, 1, 0,
                                                 0, 0, 0, 1);
    Mat prediction = KF.predict();
    printf("pre %.2f, y:%.2f\n", prediction.at<float>(0), prediction.at<float>(1));

    return 0;
}
