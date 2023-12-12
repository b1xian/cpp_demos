void mywarpPerspective(Mat src,Mat &dst,Mat T) {
    //此处注意计算模型的坐标系与Mat的不同
    //图像以左上点为（0,0），向左为x轴，向下为y轴，所以前期搜索到的特征点 存的格式是（图像x，图像y）---（rows，cols）
    //而Mat矩阵的是向下为x轴，向左为y轴，所以存的方向为（图像y，图像x）----（cols，rows）----（width，height）
    //这个是计算的时候容易弄混的
    //创建原图的四个顶点的3*4矩阵（此处我的顺序为左上，右上，左下，右下）
    Mat tmp(3, 4, CV_64FC1, 1);
    tmp.at < double >(0, 0) = 0;
    tmp.at < double >(1, 0) = 0;
    tmp.at < double >(0, 1) = src.cols;
    tmp.at < double >(1, 1) = 0;
    tmp.at < double >(0, 2) = 0;
    tmp.at < double >(1, 2) = src.rows;
    tmp.at < double >(0, 3) = src.cols;
    tmp.at < double >(1, 3) = src.rows;

    //获得原图四个顶点变换后的坐标，计算变换后的图像尺寸
    Mat corner = T * tmp;      //corner=(x,y)=(cols,rows)
    int width = 0, height = 0;
    double maxw = corner.at < double >(0, 0)/ corner.at < double >(2,0);
    double minw = corner.at < double >(0, 0)/ corner.at < double >(2,0);
    double maxh = corner.at < double >(1, 0)/ corner.at < double >(2,0);

    double minh = corner.at < double >(1, 0)/ corner.at < double >(2,0);
    for (int i = 1; i < 4; i++) {
        maxw = max(maxw, corner.at < double >(0, i)  /  corner.at < double >(2, i));
        minw  = min (minw,  corner.at < double >(0, i)  /  corner.at < double >(2, i));
        maxh  = max(maxh, corner.at < double >(1, i)  /  corner.at < double >(2, i));
        minh  = min (minh,   corner.at < double >(1, i)  /  corner.at < double >(2, i));
    }
    //创建向前映射矩阵 map_x, map_y
    //size(height,width)
    dst.create(int(maxh - minh), int(maxw - minw), src.type());
    Mat map_x(dst.size(), CV_32FC1);
    Mat map_y(dst.size(), CV_32FC1);
    Mat proj(3,1, CV_32FC1,1);
    Mat point(3,1, CV_32FC1,1);
    T.convertTo(T, CV_32FC1);
    //本句是为了令T与point同类型（同类型才可以相乘，否则报错，也可以使用T.convertTo(T, point.type() );）
    Mat Tinv=T.inv();
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            point.at<float>(0) = j + minw ;
            point.at<float>(1) = i + minh ;
            proj = Tinv * point;
            map_x.at<float>(i, j) = proj.at<float>(0)/ proj.at<float>(2) ;
            map_y.at<float>(i, j) = proj.at<float>(1) / proj.at<float>(2) ;
        }
    }
    remap(src,dst,map_x,map_y, cv::INTER_LINEAR);
}