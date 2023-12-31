#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"

using namespace cv;

static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}

void help()
{
    printf( "\nExamle of c calls to OpenCV's Kalman filter.\n"
            "   Tracking of rotating point.\n"
            "   Rotation speed is constant.\n"
            "   Both state and measurements vectors are 1D (a point angle),\n"
            "   Measurement is the real point angle + gaussian noise.\n"
            "   The real and the estimated points are connected with yellow line segment,\n"
            "   the real and the measured points are connected with red line segment.\n"
            "   (if Kalman filter works correctly,\n"
            "    the yellow segment should be shorter than the red one).\n"
            "\n"
            "   Pressing any key (except ESC) will reset the tracking with a different speed.\n"
            "   Pressing ESC will stop the program.\n"
    );
}

int main(int, char**)
{
    help();
    Mat img(500, 500, CV_8UC3);
    KalmanFilter KF(2, 1, 0);
    Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
    Mat processNoise(2, 1, CV_32F);
    Mat measurement = Mat::zeros(1, 1, CV_32F);
    char code = (char)-1;

    for(;;)
    {
        randn( state, Scalar::all(0), Scalar::all(0.1) );//随机生成一个矩阵，期望是0，标准差为0.1;
        KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);//元素导入矩阵，按行;

        //setIdentity: 缩放的单位对角矩阵;
        //!< measurement matrix (H) 观测模型
        setIdentity(KF.measurementMatrix);
        //!< process noise covariance matrix (Q)
        // wk 是过程噪声，并假定其符合均值为零，协方差矩阵为Qk(Q)的多元正态分布;
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        //!< measurement noise covariance matrix (R)
        //vk 是观测噪声，其均值为零，协方差矩阵为Rk,且服从正态分布;
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
        //!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A代表F: transitionMatrix
        //预测估计协方差矩阵;
        setIdentity(KF.errorCovPost, Scalar::all(1));
        //!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
        randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

        for(;;)
        {
            Point2f center(img.cols*0.5f, img.rows*0.5f);
            float R = img.cols/3.f;
            double stateAngle = state.at<float>(0);
            Point statePt = calcPoint(center, R, stateAngle);

            Mat prediction = KF.predict();
            double predictAngle = prediction.at<float>(0);
            Point predictPt = calcPoint(center, R, predictAngle);

            randn( measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));

            // generate measurement
            measurement += KF.measurementMatrix*state;

            double measAngle = measurement.at<float>(0);
            Point measPt = calcPoint(center, R, measAngle);

            // plot points
#define drawCross( center, color, d )                                 \
                line( img, Point( center.x - d, center.y - d ),                \
                             Point( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
                line( img, Point( center.x + d, center.y - d ),                \
                             Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

            img = Scalar::all(0);
            drawCross( statePt, Scalar(255,255,255), 3 );
//            drawCross( measPt, Scalar(0,0,255), 3 );
            drawCross( predictPt, Scalar(0,255,0), 3 );
            //line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
            //line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );

            KF.correct(measurement);

            randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
            state = KF.transitionMatrix*state + processNoise;

            imshow( "Kalman", img );
            code = (char)waitKey(100);

            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }

    return 0;
}