
#include <opencv2/opencv.hpp>



int main() {
    int dst_width=1280,dst_height=720;           // initialize the size of the output image
    cv::Mat intrinsic, distortion;
    // precompute the inverse perspective matrix and the inverse camera matrix
    cv::Mat h, k;
    cv::Mat hinv=h.inv(), kinv=k.inv();
    cv::Mat map_undist_warped_x32f(dst_height,dst_width,CV_32FC1);    // allocate the x map to the correct size (n.b. the data type used is float)
    cv::Mat map_undist_warped_y32f(dst_height,dst_width,CV_32FC1);    // allocate the y map to the correct size (n.b. the data type used is float)

    // loop on the rows of the output image
    for(int y=0; y<dst_height; ++y) {
        std::vector<cv::Point3f> pts_undist_norm(dst_width);
        // for each pixel on the current row, first use the inverse perspective mapping, 
        // then multiply by the
        // inverse camera matrix (i.e. map from pixels to normalized coordinates to prepare use of projectpoints function)
        for(int x=0; x<dst_width; ++x) {
            cv::Mat_<float> pt(3,1);
            pt << x,y,1;
            pt = kinv*hinv*pt;
            pts_undist_norm[x].x = pt(0)/pt(2);
            pts_undist_norm[x].y = pt(1)/pt(2);
            pts_undist_norm[x].z = 1;
        }
        // for each pixel on the current row,
        // compose with the inverse undistortion mapping (i.e. the distortion
        // mapping) using projectpoints function
        std::vector<cv::Point2f> pts_dist;
        cv::projectPoints(pts_undist_norm,cv::Mat::zeros(3,1,CV_32FC1),
                          cv::Mat::zeros(3,1,CV_32FC1),
                          intrinsic,
                          distortion,
                          pts_dist);
        // store the result in the appropriate pixel of the output maps
        for(int x=0; x<dst_width; ++x) {
            map_undist_warped_x32f.at<float>(y,x) = pts_dist[x].x;
            map_undist_warped_y32f.at<float>(y,x) = pts_dist[x].y;
        }
    }
    // finally, convert the float maps to signed-integer maps for best efficiency of the remap function
    cv::Mat map_undist_warped_x16s,map_undist_warped_y16s;
    cv::convertMaps(map_undist_warped_x32f,
                    map_undist_warped_y32f,
                    map_undist_warped_x16s,
                    map_undist_warped_y16s,
                    CV_16SC2);
}