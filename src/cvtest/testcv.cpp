#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//下面宏定义CV_MAT_ELEM2为方便快速访问图像像素
#define CV_MAT_ELEM2(src,dtype,y,x) \
        (dtype*)(src.data+src.step[0]*y+src.step[1]*x)

Mat polyfit(std::vector<cv::Point2f> &chain,int n)
{
    Mat y(chain.size(),1,CV_32F,Scalar::all(0));
/* ********【预声明phy超定矩阵】************************/
/* 多项式拟合的函数为多项幂函数
 * f(x)=a0+a1*x+a2*x^2+a3*x^3+......+an*x^n
 *a0、a1、a2......an是幂系数，也是拟合所求的未知量。设有m个抽样点，则：
 * 超定矩阵phy=1 x1 x1^2 ... ...  x1^n
 *           1 x2 x2^2 ... ...  x2^n
 *           1 x3 x3^2 ... ...  x3^n
 *              ... ... ... ...
 *              ... ... ... ...
 *           1 xm xm^2 ... ...  xm^n
 *
 * *************************************************/
    cv::Mat phy(chain.size(),n,CV_32F,Scalar::all(0));
    for(int i=0;i<phy.rows;i++)
    {
        float* pr=phy.ptr<float>(i);
        for(int j=0;j<phy.cols;j++)
        {
            pr[j]=pow(chain[i].x,j);
        }
        y.at<float>(i)=chain[i].y;
    }
    Mat phy_t=phy.t();
    Mat phyMULphy_t=phy.t()*phy;
    Mat phyMphyInv=phyMULphy_t.inv();
    Mat a=phyMphyInv*phy_t;
    a=a*y;
    return a;
}

int main()
{
    vector<Point2f> sp;
    //设有二次曲线点 g(x)=5+2.6x+2x^3,则:
    float a[] = {5, 2.6, 2};
    Mat image(500,500,CV_32FC1,Scalar(0));
    RNG rng;//预声明一个随机变量，用于作为离散点的干扰项
    for(int i=1; i<200; i+=2)
    {
        Point2f p;
        p.x = i;
//        for(int k = 0; k < sizeof(a); k++) {
//            p.y += a[k] * pow(i, k);//
//        }
        p.y = 5 + 2.6 * i + 2 * std::pow(i, 2);

//        p.y += rng.uniform(-1,1);//为理想点位置添加随机干扰
        /*将上面的p点以圆点的形式绘制到image上，为了观察方便，
         * 将y坐标做了颠倒，坐标原点在image的左下角*/
        Point2f pi;
        pi.x = p.x;
        pi.y = image.rows - p.y;
        circle(image, pi,3,Scalar(255),-1);
        /*-------------end--------------------*/
        sp.push_back(p);
        cout << p << endl;
    }
    image.convertTo(image,CV_8UC1);
    imshow("distributed Points",image);
    Mat am = polyfit(sp,3);
    cout << am << endl;
    waitKey();
    return 0;
}