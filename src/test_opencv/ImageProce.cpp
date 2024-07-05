
#include <opencv2/highgui.hpp>  // 包含高级GUI模块的头文件
#include <opencv2/opencv.hpp>


cv::Mat K_f120 = (cv::Mat_<double>(3, 3) << 2427.87939453125, 0.0, 1918.066650390625,
        0.0, 2437.1044921875, 1096.1136474609375,
        0.0, 0.0, 1.0);
cv::Mat D_f120 = cv::Mat({-0.31895142793655396, 0.1194758415222168, -0.03380164876580238, 0.004133735783398151});
cv::Size input_size_f120(3840, 2160);
cv::Size output_size_f120(1408, 792);

cv::Mat K_f30 = (cv::Mat_<double>(3, 3) <<
        7134.49591, 0.0, 2079.52416,
        0.0, 7107.20219, 1252.06216,
        0.0, 0.0, 1.0);
cv::Size input_size_f30(3840, 2160);
cv::Size output_size_f30(1408, 792);

class ImageProcess {
 public:
  ImageProcess() = default;
  void Init(cv::Mat K, cv::Mat D, cv::Mat new_K,
            const cv::Size& input_size, const cv::Size& output_size) {
    K_ = K;
    input_size_ = input_size;
    output_size_ = output_size;

    if (new_K.empty()) {
      cv::fisheye::estimateNewCameraMatrixForUndistortRectify(
              K, D, input_size_, cv::Matx33d::eye(), new_K_, 0.325, input_size_, 1.0);
    } else {
      new_K_ = new_K;
    }
    std::cout << K_ <<std::endl;
    std::cout << new_K_ <<std::endl;

    cv::fisheye::initUndistortRectifyMap(K, D, cv::Mat(), new_K_, input_size_, CV_32FC1, map_x_, map_y_);
  }

  void Remap(const cv::Mat& input, cv::Mat& output, int interpolation) {
    cv::remap(input, output, map_x_, map_y_, interpolation);
  }

 private:
  cv::Mat K_;
  cv::Mat D_;
  cv::Mat new_K_;
  cv::Size input_size_;
  cv::Size output_size_;
  cv::Mat map_x_;
  cv::Mat map_y_;
};



int main(int argc, char** argv) {

  // ************* f120 process *************
//  std::shared_ptr<ImageProcess> img_process_f120_;
//  img_process_f120_.reset(new ImageProcess);
//  img_process_f120_->Init(K_f120, D_f120, input_size_f120, output_size_f120);
//  cv::Mat origin_mat;
//  origin_mat = cv::imread("/home/bigaoxian/work_space/code/megvii/camera_preprocess/input/cam_front_120_origin.png");
//
//  cv::Mat remap_mat;
//  img_process_f120_->Remap(origin_mat, remap_mat, cv::INTER_LINEAR);
//  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_remap_cpp.jpg", remap_mat);
//
//  cv::Mat resize_mat;
//  cv::resize(remap_mat, resize_mat, output_size_f120);
//  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_resize_cpp.jpg", resize_mat);
//
//  cv::Mat crop_mat = resize_mat(cv::Range(712-512, 712), cv::Range(0, resize_mat.cols));
//  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_crop_cpp.jpg", crop_mat);
  // ***************************************


  // ************* f30 process *************
  std::shared_ptr<ImageProcess> img_process_f30_;
  img_process_f30_.reset(new ImageProcess);
  img_process_f30_->Init(K_f120, D_f120, K_f30, input_size_f30, output_size_f30);
  cv::Mat origin_mat_f30;
  origin_mat_f30 = cv::imread("/home/bigaoxian/work_space/code/megvii/camera_preprocess/input/cam_front_120_origin.png");

  cv::Mat remap_mat_f30;
  img_process_f30_->Remap(origin_mat_f30, remap_mat_f30, cv::INTER_LINEAR);
  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_30_remap_cpp.jpg", remap_mat_f30);

  cv::Mat resize_mat_f30;
  cv::resize(remap_mat_f30, resize_mat_f30, output_size_f30);
  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_30_resize_cpp.jpg", resize_mat_f30);

  cv::Mat crop_mat_f30 = resize_mat_f30(cv::Range(792-512, 792), cv::Range(0, resize_mat_f30.cols));
  cv::imwrite("/home/bigaoxian/work_space/code/megvii/camera_preprocess/output/cam_front_120_30_crop_cpp.jpg", crop_mat_f30);  // ***************************************
  return 0;
}
