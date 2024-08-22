//
  // Created by sheldon on 23-5-11.
//

#include <iostream>
#include <Eigen/Dense>

#include <string>
#include <vector>

const double PI = 3.1415926536;
const double ARC_TO_DEG = 180.0 / PI;
const double DEG_TO_ARC = PI / 180.0;

enum RotationUnitType { ANGLE, RADIAN };

struct Object {
  float xyz_x;
  float xyz_y;
  float lwh_l;
  float lwh_w;
  float angdir;

  void print () {
    printf("xyz_x:%.1f, xyz_y:%.1f, l:%.1f, w:%.1f, ang:%.4f\n",
           xyz_x,xyz_y,    lwh_l,    lwh_w,    angdir);
  }
};

double RfuYaw2Ego(double rfu_yaw) {
  rfu_yaw = -rfu_yaw;
  double ego_yaw = 0.;

  if (rfu_yaw >= -M_PI && rfu_yaw <= M_PI_2) {
    ego_yaw = rfu_yaw + M_PI_2;
  }

  if (rfu_yaw > M_PI_2 && rfu_yaw < M_PI) {
    ego_yaw = rfu_yaw + (-3.0 / 2.0) * M_PI;
  }
  return ego_yaw;
}

std::pair<float, float> OrientationSimilarity(double heading_radian_1, double heading_radian_2) {
    // 计算两个弧度值的差值
  float diff = fabs(heading_radian_1 - heading_radian_2);

  if (diff > M_PI) {
    diff = 2 * M_PI - diff;
  }

  return {diff, 1 - diff / M_PI};
}

Eigen::Matrix3d EulerToRotationMatrix(double roll, double pitch, double yaw,
                                                  RotationUnitType rot_unit) {
  if (rot_unit == RotationUnitType::ANGLE) {
    roll *= DEG_TO_ARC;
    pitch *= DEG_TO_ARC;
    yaw *= DEG_TO_ARC;
  }

  Eigen::Matrix3d R_x;
  R_x = Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

  Eigen::Matrix3d R_y;
  R_y = Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY());

  Eigen::Matrix3d R_z;
  R_z = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ());

  // zyx
  return R_z * R_y * R_x;
}

std::pair<Eigen::Vector3d, Eigen::Vector3d>
GetObsHeadTailPoint(Eigen::Vector3d sensor_xyz, Eigen::Vector3d lwh, double sensor_rot) {
  Eigen::Matrix3d ego2obj = EulerToRotationMatrix(0, 0, sensor_rot, RotationUnitType::RADIAN).transpose();

//  std::cout << ego2obj << std::endl;
  printf("doGetObsHeadTailPoint  center:%.1f,%.1f, lw:%.1f,%.1f, ang:%.4f\n",
         sensor_xyz[0], sensor_xyz[1], lwh[0], lwh[1], sensor_rot);
  double half_length = lwh[0] / 2;

  Eigen::Vector3d head_point;
  Eigen::Vector3d tail_point;
  head_point << 0., half_length, 0.;
  tail_point << 0., -half_length, 0.;

  head_point = ego2obj * head_point;
  tail_point = ego2obj * tail_point;
  head_point += sensor_xyz;
  tail_point += sensor_xyz;
  return {head_point, tail_point};
}

Eigen::Vector3d GetFixedCenterFromHead(const Eigen::Vector3d& sensor_xyz, const Eigen::Vector3d& lwh, double sensor_rot) {
  auto ego2obj = EulerToRotationMatrix(0, 0, sensor_rot, RotationUnitType::RADIAN).transpose();
  double half_length = lwh[0] / 2;

  Eigen::Vector3d center_point;
  center_point << 0., -half_length, 0.;

  center_point = ego2obj * center_point + sensor_xyz;
  return center_point;
}

Eigen::Vector3d GetFixedCenterFromTail(const Eigen::Vector3d& sensor_xyz, const Eigen::Vector3d& lwh, double sensor_rot) {
  auto ego2obj = EulerToRotationMatrix(0, 0, sensor_rot, RotationUnitType::RADIAN).transpose();
  double half_length = lwh[0] / 2;

  Eigen::Vector3d center_point;
  center_point << 0., half_length, 0.;

  center_point = ego2obj * center_point + sensor_xyz;
  return center_point;
}

double PointEuclideanDistance(double x1, double y1, double x2, double y2) {
  return  std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

void FixObjCenter(Object& last_obj, Object& obj) {
  float trk_length = last_obj.lwh_l;
  float curr_length = obj.lwh_l;
  float length_diff = curr_length - trk_length;

  printf("%.4f, %.4f\n", last_obj.angdir, obj.angdir);
  float yaw_ego_prev = RfuYaw2Ego(last_obj.angdir);
  float yaw_ego_curr = RfuYaw2Ego(obj.angdir);
  auto pair = OrientationSimilarity(yaw_ego_prev, yaw_ego_curr);
  float heading_diff = pair.first;
  float heading_similarity = pair.second;
  printf("yaw_ego:%.4f -> %.4f, diff:%.4f\n", yaw_ego_prev, yaw_ego_curr, heading_diff);
  if (heading_diff > 0.5) {
    return;
  }

  std::vector<float> center_prev = {last_obj.xyz_x, last_obj.xyz_y};
  std::vector<float> center_curr = {obj.xyz_x, obj.xyz_y};

  Eigen::Vector3d loc;
  loc << last_obj.xyz_x, last_obj.xyz_y, 0;
  Eigen::Vector3d lwh;
  lwh << last_obj.lwh_l, last_obj.lwh_w, 0;
  auto pair_head_tail = GetObsHeadTailPoint(loc, lwh, yaw_ego_prev);
  auto head_prev = pair_head_tail.first;
  auto tail_prev = pair_head_tail.second;

  loc << obj.xyz_x, obj.xyz_y, 0;
  lwh << obj.lwh_l, obj.lwh_w, 0;
  pair_head_tail = GetObsHeadTailPoint(loc, lwh, yaw_ego_curr);
  auto head_curr = pair_head_tail.first;
  auto tail_curr = pair_head_tail.second;

  float center_eu_dis = PointEuclideanDistance(last_obj.xyz_x, last_obj.xyz_y, obj.xyz_x, obj.xyz_y);
  float head_eu_dis = PointEuclideanDistance(head_prev[0], head_prev[1], head_curr[0], head_curr[1]);
  float tail_eu_dis = PointEuclideanDistance(tail_prev[0], tail_prev[1], tail_curr[0], tail_curr[1]);

  printf("-- origin yaw:%.4f -> %.4f, l_diff:%.1f,\n "
         "center:(%.1f,%.1f)->(%.1f,%.1f)=%.1f,\n "
         "head:(%.1f,%.1f)->(%.1f,%.1f)=%.1f, \n"
         "tail:(%.1f,%.1f)->(%.1f,%.1f)=%.1f\n",
         yaw_ego_prev, yaw_ego_curr,
         length_diff,
         center_prev[0], center_prev[1], center_curr[0], center_curr[1], center_eu_dis,
         head_prev[0], head_prev[1], head_curr[0], head_curr[1], head_eu_dis,
         tail_prev[0], tail_prev[1], tail_curr[0], tail_curr[1], tail_eu_dis);
  // correct center
  std::vector<float> diff_arr = {center_eu_dis, head_eu_dis, tail_eu_dis};
  auto min_it = std::min_element(diff_arr.begin(), diff_arr.end());
  int min_index = std::distance(diff_arr.begin(), min_it);
  // 沿用跟踪到的长度
  if (min_index == 0) {

  } else if (min_index == 1) {
    printf("fix with head point\n");
    // 头点变化最小，根据头点，和上一帧的length，修正本帧的中心点
    loc << head_curr[0], head_curr[1], 0;
    lwh << trk_length, last_obj.lwh_w, 0;
    auto fix_center = GetFixedCenterFromHead(loc, lwh, yaw_ego_curr);
    obj.xyz_x = fix_center[0];
    obj.xyz_y = fix_center[1];
    obj.lwh_l = last_obj.lwh_l;
    obj.lwh_w = last_obj.lwh_w;
  } else {
    printf("fix with tail point\n");
    // 尾点变化最小，根据尾点，和上一帧的length，修正本帧的中心点
    loc << tail_curr[0], tail_curr[1], 0;
    lwh << trk_length, last_obj.lwh_w, 0;
    auto fix_center = GetFixedCenterFromHead(loc, lwh, yaw_ego_curr);
    obj.xyz_x = fix_center[0];
    obj.xyz_y = fix_center[1];
    obj.lwh_l = last_obj.lwh_l;
    obj.lwh_w = last_obj.lwh_w;
  }


}


int main() {
  std::vector<Object> front_same_objs_data;
//  front_same_objs_data.push_back({2.3,
//                                  24.6,
//                                  4.6,
//                                  1.8,
//                                  float(PI / 2)});
//  front_same_objs_data.push_back({2.3,
//                                  27.0,
//                                  9.6,
//                                  1.8,
//                                  float(PI / 2)});
  front_same_objs_data.push_back({7.4,34.1,
                                  5.2,1.8,
                                  1.6274});

  for (size_t i = 0; i < front_same_objs_data.size(); ++i) {

    auto obj = front_same_objs_data[i];
    obj.print();

    Eigen::Vector3d loc, lwh;
    loc << obj.xyz_x, obj.xyz_y, 0;
    lwh << obj.lwh_l, obj.lwh_w, 0;
    float yaw_ego = RfuYaw2Ego(obj.angdir);
    printf("obj rfu yaw to ego:%.4f -> %.4f, center:%.1f,%.1f, lw:%.1f,%.1f\n",
           obj.angdir, yaw_ego, obj.xyz_x, obj.xyz_y, obj.lwh_l, obj.lwh_w);
    auto pair_head_tail = GetObsHeadTailPoint(loc, lwh, yaw_ego);
    const Eigen::Vector3d& curr_head = pair_head_tail.first;
    const Eigen::Vector3d& curr_tail = pair_head_tail.second;
    printf("~~~~~~~~~~~~~~GetObsHeadTailPoint: head:%.1f,%.1f, tail:%.1f,%.1f\n",
         curr_head[0],curr_head[1],
         curr_tail[0],curr_tail[1]);
//    obj rfu yaw to ego:1.6274 -> -0.0566, center:7.4,34.1, lw:5.2,1.8
//    ~~~~~~~~~~~~~~GetObsHeadTailPoint: head:0.5,36.7, tail:14.3,31.5


//    if (i != 0) {
//      auto last_obj = front_same_objs_data[i-1];
//      FixObjCenter(last_obj, obj);
//    }
//    obj.print();



  }

  return 0;
}
