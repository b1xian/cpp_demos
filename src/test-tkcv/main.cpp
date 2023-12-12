
#include <opencv2/opencv.hpp>

int   img_h = 1350;
int   img_w = 320;
float len = 150.0;
float width = 40.0;
float lane_w = 3.75; //车道宽度3.75米
float scale = 1.0;
int   h_diff = 1050;

cv::Mat InitBevImg() {
    cv::Mat world_img(img_h, img_w, CV_8UC3, cv::Scalar(255, 255, 255));
    for (int i = -100; i <= 30; i += 10) {
        cv::line(world_img, cv::Point(img_w, img_h-h_diff-(i/len)*img_h), cv::Point(0, img_h-h_diff-(i/len)*img_h),
                 cv::Scalar(255,191,0), 1);
        std::string dis_str = std::to_string(i);
        cv::putText(world_img, dis_str,
                    cv::Point(img_w - 100, img_h-h_diff - (i/len)*img_h),
                    cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,0,255), 1);
    }

    cv::line(world_img, cv::Point((width/2+lane_w/2)/width*img_w, 0), cv::Point((width/2+lane_w/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);
    cv::line(world_img, cv::Point((width/2-lane_w/2)/width*img_w, 0), cv::Point((width/2-lane_w/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);
    cv::line(world_img, cv::Point((width/2+lane_w*3/2)/width*img_w, 0), cv::Point((width/2+lane_w*3/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);
    cv::line(world_img, cv::Point((width/2-lane_w*3/2)/width*img_w, 0), cv::Point((width/2-lane_w*3/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);
    cv::line(world_img, cv::Point((width/2+lane_w*5/2)/width*img_w, 0), cv::Point((width/2+lane_w*5/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);
    cv::line(world_img, cv::Point((width/2-lane_w*5/2)/width*img_w, 0), cv::Point((width/2-lane_w*5/2)/width*img_w, img_h),
             cv::Scalar(128,128,128), 2);

    return world_img;
}

cv::RotatedRect GetObjRotateBox(cv::Point2f center, cv::Size2f size, float heading) {
    double obj_x = center.x;
    double obj_y = center.y;
    float obj_l = size.height;
    float obj_w = size.width;

    float rot_rect_center_x = img_w / 2 * scale + -obj_y / width * img_w  * scale;
    float rot_rect_center_y = (img_h - h_diff) - obj_x / len * img_h * scale;
    cv::Point2f rot_rect_center(rot_rect_center_x, rot_rect_center_y);
    float rot_rect_w = (obj_w / width * img_w) * scale;
    float rot_rect_h = (obj_l / len * img_h) * scale;
    cv::Size2f rot_rect_size(rot_rect_h, rot_rect_w);
    float rot_rect_angle = ((-heading + M_PI / 2) * (180 / M_PI));
    cv::RotatedRect rRect = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);
    return rRect;
}

cv::RotatedRect GetObjRotateBoxOrigin(cv::Point2f center, cv::Size2f size, float heading) {
    cv::Point2f rot_rect_center(center.y, center.x);
    cv::Size2f rot_rect_size(size.height, size.width);
    float rot_rect_angle = ((heading + M_PI / 2) * (180 / M_PI));
    cv::RotatedRect rRect = cv::RotatedRect(rot_rect_center, rot_rect_size, rot_rect_angle);
    return rRect;
}

void DrawObj(cv::RotatedRect& rect, cv::Mat& bev_img, const cv::Scalar& color) {
    cv::Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            cv::line(bev_img, vertices[i], vertices[(i + 1) % 4], color, 4);
        } else {
            cv::line(bev_img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0));
        }
    }
}

int find_leftmost_point(std::vector<cv::Point2f> intersectingRegion) {
    int index = 0;
    float tmp = intersectingRegion[0].x;
    for (int i = 1; i < intersectingRegion.size(); i++) {
        if (intersectingRegion[i].x < tmp) {
            tmp = intersectingRegion[i].x;
            index = i;
        }
    }
    return index;
}

std::vector<cv::Point2f> sort_points(std::vector<cv::Point2f> intersectingRegion)
{
    std::vector<cv::Point2f> sort_intersectingRegion;
    int leftmost_index = find_leftmost_point(intersectingRegion);

    std::vector<float> arctan;
    for(int i=0; i<intersectingRegion.size(); i++)
    {
        arctan.push_back(atan2(intersectingRegion[i].y - intersectingRegion[leftmost_index].y, intersectingRegion[i].x - intersectingRegion[leftmost_index].x));
    }

    std::vector<int> index;
    for(int i=0; i<arctan.size(); i++)
    {
        index.push_back(i);
    }

    sort(index.begin(), index.end(), [&](const int& a, const int& b) {return (arctan[a] < arctan[b]);});

    for(int i=0; i<index.size(); i++)
    {
        sort_intersectingRegion.push_back(intersectingRegion[index[i]]);
    }
    return sort_intersectingRegion;
}

// 计算两个旋转矩形的 IoU
float calculateIoU(const cv::RotatedRect& rect1, const cv::RotatedRect& rect2) {
    cv::Rect boundingBox1 = rect1.boundingRect();
    cv::Rect boundingBox2 = rect2.boundingRect();

    cv::Rect intersection = boundingBox1 & boundingBox2;
    float interArea = intersection.area();
    float unionAreaVal = boundingBox1.area() + boundingBox2.area() - interArea;

    // 避免除以零
    if (unionAreaVal == 0) {
        return 0.0f;
    }

    return interArea / unionAreaVal;
}

float getIOU(const cv::Rect_<float>& bb_first, const cv::Rect_<float>& bb_second) {
    float in = (bb_first & bb_second).area();
    float un = (bb_first | bb_second).area();
    if (un < DBL_EPSILON) {
        return 0;
    } else {
        return in / un;
    }
}


int main(int argc, char** argv) {
    std::cout << "test tk cv" << std::endl;
    cv::Mat bev_img = InitBevImg();

    float self_center_x = 0.0;
    float self_center_y = 0.0;
    float self_length = 12.0;
    float self_width = 3.0;
    float self_heading = 0.0;
    cv::RotatedRect self_rect = GetObjRotateBox(cv::Point2f(self_center_x, self_center_y),
                                                cv::Size2f(self_width, self_length), self_heading);

    float obj_center_x = 4.817339;
    float obj_center_y = -4.22;
    float obj_length = 7.47114;
    float obj_width = 2.40591;
    float obj_heading = 0.40360168;
    cv::RotatedRect obj_rect = GetObjRotateBox(cv::Point2f(obj_center_x, obj_center_y),
                                               cv::Size2f(obj_width, obj_length), obj_heading);
    DrawObj(self_rect, bev_img, cv::Scalar(255, 0, 0));
    DrawObj(obj_rect, bev_img, cv::Scalar(0, 0, 255));
    cv::Rect_<float> bond_rect = obj_rect.boundingRect2f();
    std::cout << "obj bond_rect: " << bond_rect << std::endl;
    cv::rectangle(bev_img, cv::Point2i(bond_rect.tl().x, bond_rect.tl().y),
                  cv::Point2i(bond_rect.br().x, bond_rect.br().y), (0, 255,255), 1);

    std::vector<cv::Point2f> intersectingRegion;
    cv::rotatedRectangleIntersection(self_rect, obj_rect, intersectingRegion);
    float area_r1 = self_rect.size.width * self_rect.size.height;
    float area_r2 = obj_rect.size.width * obj_rect.size.height;
    float inter_area = 0.f;
    if (!intersectingRegion.empty()) {
        std::cout << intersectingRegion << std::endl;
        for (int i = 0; i < intersectingRegion.size(); i++) {
            cv::circle(bev_img, intersectingRegion[i], 3, cv::Scalar(0, 0, 0));
            cv::putText(bev_img, std::to_string(i), intersectingRegion[i], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        }
        std::cout << intersectingRegion << std::endl;
        // need to sort the vertices CW or CCW
        std::vector<cv::Point2f> intersections;
        cv::convexHull(intersectingRegion, intersections);
        for (unsigned int i = 0; i < intersections.size(); ++i) {
            const auto& pt = intersections[i];
            const unsigned int i_next = (i + 1) == intersections.size() ? 0 : (i + 1);
            const auto& pt_next = intersections[i_next];
            inter_area += (pt.x * pt_next.y - pt_next.x * pt.y);
        }
        inter_area = std::abs(inter_area) / 2;
        std::cout << "inter!!! area: " << inter_area << std::endl;
    } else {
        std::cout << "not inter" << std::endl;
    }
    float iou = inter_area / (area_r1 + area_r2 - inter_area + 0.00000001f);
    //float iou = calculateIoU(self_rect, obj_rect);
    std::cout << "area1: " << area_r1 << ", area2: " << area_r2 << ", inter_area: " << inter_area << ", iou: " << iou << std::endl;


    std::cout << "-------------------------------" << std::endl;
    self_rect = GetObjRotateBoxOrigin(cv::Point2f(self_center_x, -self_center_y),
                                    cv::Size2f(self_width, self_length), self_heading);
    obj_rect = GetObjRotateBoxOrigin(cv::Point2f(obj_center_x, -obj_center_y),
                                    cv::Size2f(obj_width, obj_length), obj_heading);
    std::cout << "origin self: " << self_rect.boundingRect2f().tl() << ", " << self_rect.boundingRect2f().br() << std::endl;
    std::cout << "origin obj : " << obj_rect.boundingRect2f().tl() << ", " << obj_rect.boundingRect2f().br() << std::endl;
    cv::Point2f vertices[4];
    self_rect.points(vertices);
    for (int i = 0; i < 4; i++) {
        std::cout << "* self point: " << vertices[i] << std::endl;
    }
    obj_rect.points(vertices);
    for (int i = 0; i < 4; i++) {
        std::cout << "-- obj point: " << vertices[i] << std::endl;
    }
    area_r1 = self_rect.size.width * self_rect.size.height;
    area_r2 = obj_rect.size.width * obj_rect.size.height;
    inter_area = 0.f;
    cv::rotatedRectangleIntersection(self_rect, obj_rect, intersectingRegion);
    if (!intersectingRegion.empty()) {
        std::cout << intersectingRegion << std::endl;
        // need to sort the vertices CW or CCW
        std::vector<cv::Point2f> intersections;
        cv::convexHull(intersectingRegion, intersections);
        for (unsigned int i = 0; i < intersections.size(); ++i) {
            const auto& pt = intersections[i];
            const unsigned int i_next = (i + 1) == intersections.size() ? 0 : (i + 1);
            const auto& pt_next = intersections[i_next];
            inter_area += (pt.x * pt_next.y - pt_next.x * pt.y);
        }
        inter_area = std::abs(inter_area) / 2;
        std::cout << "origin inter!!! area: " << inter_area << std::endl;
    } else {
        std::cout << "origin not inter" << std::endl;
    }
    iou = inter_area / (area_r1 + area_r2 - inter_area + 0.00000001f);
    std::cout << "origin area1: " << area_r1 << ", area2: " << area_r2 << ", inter_area: " << inter_area << ", iou: " << iou << std::endl;
    float rect_iou = getIOU(self_rect.boundingRect(), obj_rect.boundingRect());
    std::cout << "rect iou: " << rect_iou << std::endl;


    cv::imshow("bev_img", bev_img);
    cv::waitKey(0);

    return 0;
}
