#ifndef DEVASTATOR_PERCEPTION_CAMERA_OBSTACLE_DRAW_UTIL_H
#define DEVASTATOR_PERCEPTION_CAMERA_OBSTACLE_DRAW_UTIL_H

#include "object.hpp"
#include <opencv2/opencv.hpp>


class DrawUtil {
 public:
  static void DrawBevDebugImage(cv::Mat& bev_debug_img, const std::vector<ObjectPtr>& obj_list);

  static cv::Rect RescaleRect(const cv::Rect& rect, float scale);

  static void DrawBevLines(cv::Mat& world_img);

  static void DrawBevObstacles(cv::Mat& bev_img,
                               const std::vector<ObjectPtr>& obj_list);

  static void DrawBevRect(cv::Mat& bev_img, const cv::RotatedRect& rRect,
                          cv::Scalar head_color);

  static void DrawBevObstacleHeading(cv::Mat& bev_img, const cv::Point2f& center,
                                     double length, double det_heading,
                                     cv::Scalar color);

  static cv::RotatedRect GetObjRotateBox(float obj_x, float obj_y, float obj_l, float obj_w, float obj_yaw);

 public:
  static float lane_w;
  static float front_len;
  static float back_len;
  static float len;
  static float width;
  static float scale;
  static int img_h;
  static int img_w;
  static int img_zero_h;
  static cv::Scalar red_color;
  static cv::Scalar blue_color;
  static cv::Scalar green_color;
  static cv::Scalar yellow_color;
};

#endif //DEVASTATOR_PERCEPTION_CAMERA_OBSTACLE_DRAW_UTIL_H
