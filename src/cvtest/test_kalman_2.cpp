#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<stdio.h>
#include<opencv2/video/tracking.hpp>
using namespace std;
using namespace cv;
//using namespace cs;

static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}

static void help()
{
    printf("opencv里面的卡尔曼滤波示例\n"
           "   跟踪一个不断旋转的点：用卡尔曼滤波预测当前点的位置.\n"
           "   旋转速度是固定的.\n"
           "   状态向量和观测向量都是1维的，就是被跟踪的点的圆心角,\n"
           "   角度的观测值等于真实的角度值加上高斯噪声.\n"
           "   点的真实位置和估计位置用黄色的线段连起来,\n"
           "   点的真实位置和观测位置用红色的线段连起来.\n"
           "   (如果kalman滤波器的工作是正常的,\n"
           "    黄的线段应该比红色线段短).\n"
           "\n"
           "   除了ESC键之外，按下任何键都会使得点的速度加快.\n"
           "   按ESC键退出程序.\n"
    );
}
int main(int,char**)
{
    help();
    Mat img(500, 500,CV_8UC3);          //定义了图像的大小，三通道无符八位char
    cv::KalmanFilter KF(2, 1, 0);       //声明了一个kalman滤波器的类  在cs命名空间
    Mat state(2, 1, CV_32F);           //状态矩阵 里面包含两个状态变量(角度值，角度值的增量)
    Mat processNoise(2, 1, CV_32F);    //过程噪声矩阵;
    Mat measurement = Mat::zeros(1, 1, CV_32F);  //真实观测值
    char code = (char)-1;

    for (;;)
    {
        randn( state, Scalar::all(0), Scalar::all(0.1));           //产生高斯分布的随机状态向量均值为0，标准差为0.1
        cout << state<<endl;
        KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);   //转移矩阵(A)[1 1;0 1]
        cout << KF.transitionMatrix<<endl;
        setIdentity(KF.measurementMatrix);                        //观测矩阵赋值
        cout << KF.measurementMatrix<<endl;
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));         //给过程噪声协方差矩阵赋值，对角元素大小为1e-5
        cout << KF.processNoiseCov<<endl;
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));     //给观测噪声协方差矩阵赋值，对角元素大小为1e-1
        cout << KF.measurementNoiseCov<<endl;
        setIdentity(KF.errorCovPost, Scalar::all(1));               //给前验估计值的误差协方差矩阵赋值，对角元素大小为1
        cout << KF.errorCovPost<<endl;
        randn(KF.statePost, Scalar::all(0), Scalar::all(0));        //后验状态估计值赋值  均值=0，方差=0
        cout << KF.statePost<<endl;

        for (;;)
        {
            Point2f center(img.cols*0.5f, img.rows*0.5f);                    //图像中心点
            float R = img.cols / 3.f;                                     //半径
            double stateangle = state.at<float>(0);                         //状态中的角度值
            Point statePt = calcPoint(center, R, stateangle);               //将角度值转换为圆周上的坐标Rsin(angle),-Rcos(angle)

            //预测,得到当前时刻状态的先验估计值
            Mat prediction = KF.predict();                                  //将上一时刻的后验估计值转移到这一时刻
            cout << prediction<<endl;
            double predictangle = prediction.at<float>(0);                    //取出先验估计的角度值
            cout << predictangle<<endl;
            Point predictPt = calcPoint(center, R, predictangle);             //将先验估计的角度值转化为坐标

            //更新
            randn(measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));  //生成当前观测值中的噪声
            cout << measurement<<endl;
            //当前时刻的观测值， Z=H*X+R  观测噪声R属于N(0,1e-1)分布，所以z=angle+N(0,1e-1)
            measurement += KF.measurementMatrix*state;
            cout << measurement<<endl;
            double meanAngle = measurement.at<float>(0);                   //取出观测值中的角度值
            cout << meanAngle<<endl;
            Point measPt = calcPoint(center, R, meanAngle);                //计算观测值中的角度值对应的圆周坐标点

            //定义了划十字的方法，

#define drawCross( center, color, d )                                 \
                line( img, Point( center.x - d, center.y - d ),                \
                             Point( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
                line( img, Point( center.x + d, center.y - d ),                \
                             Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )


            img = Scalar::all(0);                                        //BGR
            drawCross(statePt,Scalar(255, 255, 255),3);                  //白色的点是上一时刻点在圆周的位置
            drawCross(measPt, Scalar(0, 0, 255), 3);                     //红色的点是当前时刻观测到的点在圆周上的位置
            drawCross(predictPt, Scalar(0, 255, 0), 3);                  //绿色的点是当前时刻在圆周上位置的先验估计得到的点
            //	line(img, statePt, measPt, Scalar(0, 0, 255), 3, CV_AA, 0);   //从上一时刻的位置到当前观测的位置的连线
            //	line(img, statePt, predictPt, Scalar(0, 255, 255), 3, CV_AA, 0); //从上一时刻的位置到当前一步预测的位置的连线

            //利用观测值去修正先验估计值，得到真正的后验估计值
            if (theRNG().uniform(0, 4) != 0)
                KF.correct(measurement);

            //不加噪声的话就是匀速圆周运动，加了点噪声类似匀速圆周运动，因为噪声的原因，运动方向可能会改变
            randn(processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
            cout << processNoise<<endl;
            // 产生过程噪声，过程噪声属于二维高斯分布，每一维的均值为0 方差为1e-5
            state = KF.transitionMatrix*state + processNoise;
            cout << state<<endl;
            //产生下一时刻的状态
            imshow("Kalman", img);
            //code = (char)waitKey(100);
            waitKey(100);
            //	if (code > 0)
            //		break;
        }
        if (code == 27 || code == 'q' || code == 'Q')
            break;
    }
    return 0;
}