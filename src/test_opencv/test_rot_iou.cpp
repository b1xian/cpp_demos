#include "DrawUtil.h"

float Rect2DIou(const cv::RotatedRect& rect1, const cv::RotatedRect& rect2) {
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

int main() {

//  bev 46(425,425,0,y,y,y), l_loc:(3.0,48.2), d_loc:(3.0,48.3), c_loc:(3.0,48.3), move:0.0, vx:-0.26, vy:21.5, acc:-0.0,0.1,stddev:(loc:0.0,0.2, velo:0.0,0.2)
//  bev 48(217,216,0,y,y,y), l_loc:(-1.0,50.1), d_loc:(-1.0,49.2), c_loc:(-1.0,49.5), move:0.6, vx:-0.20, vy:18.4, acc:-0.0,0.1,stddev:(loc:0.0,0.6, velo:0.0,0.1)
  cv::Mat bev_debug_img;
  std::vector<ObjectPtr> obj_list;
  ObjectPtr obj1;
  obj1.reset(new Object());
  obj1->sensor_xyz = {3.0, 48.2, 0};
  obj1->lwh = {4.6, 1.8, 2.0};
  obj1->sensor_rot = 1.560;
  obj_list.push_back(obj1);

  ObjectPtr obj2;
  obj2.reset(new Object());
  obj2->sensor_xyz = {-1.0, 48.1, 0};
  obj2->lwh = {4.6, 1.8, 2.0};
  obj2->sensor_rot = 1.560;
  obj_list.push_back(obj2);

  DrawUtil::DrawBevDebugImage(bev_debug_img, obj_list);

//  DrawUtil::GetObjRotateBox(obj2->sensor_xyz[1], obj2->sensor_xyz[0], obj2->lwh[1], obj2->lwh[0], obj2->sensor_rot);

  float rot_rect_center_x = DrawUtil::img_w / 2  + ((obj2->sensor_xyz[0] / (DrawUtil::width / 2)) * (DrawUtil::img_w / 2));
  float rot_rect_center_y = DrawUtil::img_zero_h - (obj2->sensor_xyz[1] / DrawUtil::len) * DrawUtil::img_h;
  float rot_rect_w = (obj2->lwh[0] / DrawUtil::width * DrawUtil::img_w);
  float rot_rect_h = (obj2->lwh[1] / DrawUtil::len * DrawUtil::img_h);
  cv::Size2f rot_rect_size(rot_rect_h, rot_rect_w);
  cv::Point2f rot_rect_center(rot_rect_center_x, rot_rect_center_y);
  float rot_rect_angle = obj2->sensor_rot * 180 / CV_PI;
  rot_rect_angle = ((-obj2->sensor_rot + M_PI / 2) * (180 / M_PI));
  cv::RotatedRect target_2_rect_bev(rot_rect_center, rot_rect_size, rot_rect_angle);
  DrawUtil::DrawBevRect(bev_debug_img, target_2_rect_bev, DrawUtil::red_color);

  rot_rect_center_x = DrawUtil::img_w / 2  + ((obj1->sensor_xyz[0] / (DrawUtil::width / 2)) * (DrawUtil::img_w / 2));
  rot_rect_center_y = DrawUtil::img_zero_h - (obj1->sensor_xyz[1] / DrawUtil::len) * DrawUtil::img_h;
  rot_rect_w = (obj1->lwh[0] / DrawUtil::width * DrawUtil::img_w);
  rot_rect_h = (obj1->lwh[1] / DrawUtil::len * DrawUtil::img_h);
//  rot_rect_center_x = obj1->sensor_xyz[0];
//  rot_rect_center_y = obj1->sensor_xyz[1];
//  rot_rect_w = obj1->lwh[0];
//  rot_rect_h = obj1->lwh[1];
  rot_rect_center = cv::Point2f(rot_rect_center_x, rot_rect_center_y);
  rot_rect_size = cv::Size2f(rot_rect_h, rot_rect_w);
  rot_rect_angle = obj1->sensor_rot * 180 / CV_PI;
  rot_rect_angle = ((-obj1->sensor_rot + M_PI / 2) * (180 / M_PI));
  cv::RotatedRect target_1_rect_bev(rot_rect_center, rot_rect_size, rot_rect_angle);
  DrawUtil::DrawBevRect(bev_debug_img, target_1_rect_bev, DrawUtil::red_color);

  float resize_iou = Rect2DIou(target_1_rect_bev, target_2_rect_bev);
  printf("bev resize iou = %f\n", resize_iou);

  rot_rect_center_x = obj2->sensor_xyz[0];
  rot_rect_center_y = obj2->sensor_xyz[1];
  rot_rect_w = obj2->lwh[0];
  rot_rect_h = obj2->lwh[1];
  rot_rect_center = cv::Point2f(rot_rect_center_x, rot_rect_center_y);
  rot_rect_size = cv::Size2f(rot_rect_h, rot_rect_w);
  target_2_rect_bev = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);

  rot_rect_center_x = obj1->sensor_xyz[0];
  rot_rect_center_y = obj1->sensor_xyz[1];
  rot_rect_w = obj1->lwh[0];
  rot_rect_h = obj1->lwh[1];
  rot_rect_center = cv::Point2f(rot_rect_center_x, rot_rect_center_y);
  rot_rect_size = cv::Size2f(rot_rect_h, rot_rect_w);
  target_1_rect_bev = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);

  float origin_iou = Rect2DIou(target_1_rect_bev, target_2_rect_bev);
  printf("bev origin iou = %f\n", origin_iou);

  rot_rect_center_x = obj2->sensor_xyz[0];
  rot_rect_center_y = obj2->sensor_xyz[1];
  rot_rect_w = obj2->lwh[1];
  rot_rect_h = obj2->lwh[0];
  rot_rect_angle = obj2->sensor_rot * 180 / CV_PI;
  rot_rect_center = cv::Point2f(rot_rect_center_x, rot_rect_center_y);
  rot_rect_size = cv::Size2f(rot_rect_h, rot_rect_w);
  target_2_rect_bev = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);

  rot_rect_center_x = obj1->sensor_xyz[0];
  rot_rect_center_y = obj1->sensor_xyz[1];
  rot_rect_w = obj1->lwh[1];
  rot_rect_h = obj1->lwh[0];
  rot_rect_angle = obj1->sensor_rot * 180 / CV_PI;
  rot_rect_center = cv::Point2f(rot_rect_center_x, rot_rect_center_y);
  rot_rect_size = cv::Size2f(rot_rect_h, rot_rect_w);
  target_1_rect_bev = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);

  float test_iou = Rect2DIou(target_1_rect_bev, target_2_rect_bev);
  printf("test_iou iou = %f\n", origin_iou);

  cv::putText(bev_debug_img, "origin_iou:"+ std::to_string(origin_iou),
              cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);
  cv::putText(bev_debug_img, "resize_iou:"+ std::to_string(resize_iou),
              cv::Point(20, 60), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);
  cv::putText(bev_debug_img, "test_iou:"+ std::to_string(test_iou),
              cv::Point(20, 100), cv::FONT_HERSHEY_COMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);
  cv::imwrite("/home/bigaoxian/work_space/code/dev_code/cpp_demos/src/test_opencv/bev_debug_img.jpg", bev_debug_img.clone());
  cv::imshow("bev_debug_img", bev_debug_img);
  int key = cv::waitKey(33);
  // wait until press 'q'
  while(key != 'q') {
    key = cv::waitKey(33);
  }

  // iou = 0.064012
  // iou = 0.067310
  return 0;
}