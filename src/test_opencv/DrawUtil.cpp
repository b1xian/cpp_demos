#include "DrawUtil.h"

#include <future>



float DrawUtil::lane_w = 3.75; //车道宽度3.75米
float DrawUtil::front_len = 180.0; // 前180m
float DrawUtil::back_len = 80.0; // 后80m
float DrawUtil::len = front_len + back_len;
float DrawUtil::width = 45.0; // 左右各45m
int DrawUtil::img_h = 1600; // 高
float DrawUtil::scale = img_h / len; // pixel in 1m
int DrawUtil::img_w = width * scale; // 宽
int DrawUtil::img_zero_h = front_len * scale; // 0m位置高度

cv::Scalar DrawUtil::red_color = cv::Scalar(0, 0, 255);
cv::Scalar DrawUtil::blue_color = cv::Scalar(255, 0, 0);
cv::Scalar DrawUtil::green_color = cv::Scalar(0, 255, 0);
cv::Scalar DrawUtil::yellow_color = cv::Scalar(0, 255, 255);

void DrawUtil::DrawBevDebugImage(cv::Mat& bev_debug_img, const std::vector<ObjectPtr>& obj_list) {
  if (bev_debug_img.empty()) {
    bev_debug_img.create(img_h, img_w, CV_8UC3);
  }
  bev_debug_img.setTo(cv::Scalar(0, 0, 0));

  DrawBevLines(bev_debug_img);

  DrawBevObstacles(bev_debug_img, obj_list);
}


void DrawUtil::DrawBevObstacles(cv::Mat& bev_img,
                                const std::vector<ObjectPtr>& obj_list) {
  for (const auto &object : obj_list) {
    double obj_x = object->sensor_xyz[1];
    double obj_y = object->sensor_xyz[0];
    float obj_l = object->lwh[1];
    float obj_w = object->lwh[0];
    double det_heading = object->sensor_rot;

    cv::RotatedRect rRect = GetObjRotateBox(obj_x, obj_y, obj_l, obj_w, det_heading);
    DrawBevRect(bev_img, rRect, green_color);
    DrawBevObstacleHeading(bev_img, rRect.center, obj_w, det_heading, green_color);
  }
}

void DrawUtil::DrawBevRect(cv::Mat& bev_img, const cv::RotatedRect& rRect,
                           cv::Scalar head_color) {
  cv::Point2f vertices[4];
  rRect.points(vertices);
  for (int i = 0; i < 4; i++) {
    if (i == 1) {
      cv::line(bev_img, vertices[i], vertices[(i + 1) % 4], head_color);
    } else {
      cv::line(bev_img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 255, 255));
    }
  }
}

cv::RotatedRect DrawUtil::GetObjRotateBox(float obj_x, float obj_y, float obj_l, float obj_w, float obj_yaw) {
//  double obj_x = object->sensor_xyz[1];
//  double obj_y = object->sensor_xyz[0];
  float rot_rect_center_x = img_w / 2  + ((obj_y / (width / 2)) * (img_w / 2));
  float rot_rect_center_y = img_zero_h - (obj_x / len) * img_h;
  cv::Point2f rot_rect_center(rot_rect_center_x, rot_rect_center_y);

//  float obj_l = object->lwh[1];
//  float obj_w = object->lwh[0];
  float rot_rect_w = (obj_w / width * img_w);
  float rot_rect_h = (obj_l / len * img_h);
  cv::Size2f rot_rect_size(rot_rect_h, rot_rect_w);

  // 转换至图像角度
  float rot_rect_angle = ((-obj_yaw + M_PI / 2) * (180 / M_PI));
  cv::RotatedRect rRect = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);
  return rRect;
}

void DrawUtil::DrawBevObstacleHeading(cv::Mat& bev_img, const cv::Point2f& center,
                                      double length, double det_heading,
                                      cv::Scalar color) {
  double heading_rad = -det_heading + M_PI / 2;
  float extend_c = length * 1.2 * scale;
  float y_end = center.y - cos(heading_rad) * extend_c;
  float x_end = center.x + sin(heading_rad) * extend_c;
//  printf("~~~~~det_heading:%.4f, center_x:%.1f, y:%.1f, end x:%.1f, y:%.1f\n",
//         heading_rad, center.x, center.y, x_end, y_end);
  cv::line(bev_img, center, cv::Point(x_end, y_end), color, 1);
//  std::string det_heading_str{StringUtil::Float2RoundString(heading_rad, 4)};
//  cv::putText(bev_img, det_heading_str, cv::Point(x_end, y_end),
//              cv::FONT_HERSHEY_PLAIN, 1, color, 1);
}

void DrawUtil::DrawBevLines(cv::Mat& bev_img) {
  // 纵向距离标识线
  for (int i = -back_len; i <= front_len; i += 10) {
    int line_h = img_zero_h - i / len * img_h;
    cv::line(bev_img,
             cv::Point(0, line_h), cv::Point(img_w, line_h),
             cv::Scalar(255,191,0), 1);
    std::string dis_str = std::to_string(i);
    cv::putText(bev_img, dis_str, cv::Point(img_w - 60, line_h),
                cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,0,255), 1);
  }

  // 横向车道标识线
  int left_1_lane_w = (width - lane_w) / 2 / width * img_w;
  int left_2_lane_w = (width - lane_w * 3) / 2 / width * img_w;
  int left_3_lane_w = (width - lane_w * 5) / 2 / width * img_w;
  cv::line(bev_img, cv::Point(left_1_lane_w, 0), cv::Point(left_1_lane_w, img_h), cv::Scalar(128,128,128), 2);
  cv::line(bev_img, cv::Point(left_2_lane_w, 0), cv::Point(left_2_lane_w, img_h), cv::Scalar(128,128,128), 2);
  cv::line(bev_img, cv::Point(left_3_lane_w, 0), cv::Point(left_3_lane_w, img_h), cv::Scalar(128,128,128), 2);
  int right_2_lane_w = (width + lane_w * 3) / 2 / width * img_w;
  int right_1_lane_w = (width + lane_w) / 2 / width * img_w;
  int right_3_lane_w = (width + lane_w * 5) / 2 / width * img_w;
  cv::line(bev_img, cv::Point(right_1_lane_w, 0), cv::Point(right_1_lane_w, img_h), cv::Scalar(128,128,128), 2);
  cv::line(bev_img, cv::Point(right_2_lane_w, 0), cv::Point(right_2_lane_w, img_h), cv::Scalar(128,128,128), 2);
  cv::line(bev_img, cv::Point(right_3_lane_w, 0), cv::Point(right_3_lane_w, img_h), cv::Scalar(128,128,128), 2);

  // 自车
  float vehicle_w = 2;
  float vehicle_l = 5;
  int img_mid_w = img_w/2;
  int vehicle_left_w = img_mid_w-(vehicle_w/width*img_mid_w);
  int vehicle_right_w = img_mid_w+(vehicle_w/width*img_mid_w);
  int vehicle_top_h = img_zero_h-vehicle_l/len*img_h;
  int vehicle_bottom_h = img_zero_h;
  cv::line(bev_img, cv::Point(vehicle_left_w, vehicle_top_h), cv::Point(vehicle_right_w, vehicle_top_h), cv::Scalar(0,255,255), 2);
  cv::line(bev_img, cv::Point(vehicle_right_w, vehicle_top_h), cv::Point(vehicle_right_w, vehicle_bottom_h), cv::Scalar(0,255,255), 2);
  cv::line(bev_img, cv::Point(vehicle_right_w, vehicle_bottom_h), cv::Point(vehicle_left_w, vehicle_bottom_h), cv::Scalar(0,255,255), 2);
  cv::line(bev_img, cv::Point(vehicle_left_w, vehicle_bottom_h), cv::Point(vehicle_left_w, vehicle_top_h), cv::Scalar(0,255,255), 2);
}


cv::Rect DrawUtil::RescaleRect(const cv::Rect& rect, float scale) {
  cv::Rect rescaled_rect;
  rescaled_rect.x = int(rect.x * scale);
  rescaled_rect.y = int(rect.y * scale);
  rescaled_rect.width = int(rect.width * scale);
  rescaled_rect.height = int(rect.height * scale);
  return rescaled_rect;
}


