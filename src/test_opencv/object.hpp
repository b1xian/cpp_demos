#ifndef DEVASTATOR_PERCEPTION_FUSION_BASE_OBJECT_H
#define DEVASTATOR_PERCEPTION_FUSION_BASE_OBJECT_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <map>
#include <memory>
#include <string>

#include <opencv2/opencv.hpp>


enum class ObjectType {
  CAR = 1,
  TRUCK = 2,
  CONSTRUCTION_VEHICLE = 3,
  BUS = 4,
  MOTORCYCLE = 5,
  BICYCLE = 6,
  TRICYCLE = 7,
  CYCLIST = 8,
  PEDESTRIAN = 9
};

enum class ObjectDirection {
  UNKNOWN = 0,
  SAME = 1,
  OPPOSITE = 2
};

// object type mapping
const std::map<ObjectType, std::string> kObjectType2NameMap = {
    {ObjectType::CAR, "CAR"},
    {ObjectType::TRUCK, "TRUCK"},
    {ObjectType::CONSTRUCTION_VEHICLE, "CONSTRUCTION_VEHICLE"},
    {ObjectType::BUS, "BUS"},
    {ObjectType::MOTORCYCLE, "MOTORCYCLE"},
    {ObjectType::BICYCLE, "BICYCLE"},
    {ObjectType::TRICYCLE, "TRICYCLE"},
    {ObjectType::CYCLIST, "CYCLIST"},
    {ObjectType::PEDESTRIAN, "PEDESTRIAN"}};

const std::map<std::string, ObjectType> kObjectName2TypeMap = {
    {"CAR", ObjectType::CAR},
    {"TRUCK", ObjectType::TRUCK},
    {"CONSTRUCTION_VEHICLE", ObjectType::CONSTRUCTION_VEHICLE},
    {"BUS", ObjectType::BUS},
    {"MOTORCYCLE", ObjectType::MOTORCYCLE},
    {"BICYCLE", ObjectType::BICYCLE},
    {"TRICYCLE", ObjectType::TRICYCLE},
    {"CYCLIST", ObjectType::CYCLIST},
    {"PEDESTRIAN", ObjectType::PEDESTRIAN}};

const std::map<ObjectType, cv::Scalar> kObjectType2Color = {
    {ObjectType::CAR, {255, 0, 0}},
    {ObjectType::TRUCK, {255, 127, 0}},
    {ObjectType::CONSTRUCTION_VEHICLE, {255, 255, 0}},
    {ObjectType::BUS, {0, 255, 0}},
    {ObjectType::MOTORCYCLE, {0, 255, 128}},
    {ObjectType::BICYCLE, {0, 0, 255}},
    {ObjectType::TRICYCLE, {128, 75, 0}},
    {ObjectType::CYCLIST, {75, 0, 130}},
    {ObjectType::PEDESTRIAN, {148, 0, 211}}};

struct Object {
  Object() = default;

  Object(const Eigen::Vector3d& sensor_p, const Eigen::Quaterniond& sensor_ori,
         const Eigen::Vector3d& s, const Eigen::Affine3d& sensor2global_trans);
  /**
   * @brief Get the Affine matrix
   *
   * @param rot rotation angle
   * @param xyz tranlation coordinates
   * @return Eigen::Affine3d
   */
  Eigen::Affine3d GetAffine(const Eigen::Quaterniond& orientation, const Eigen::Vector3d& xyz);

  /**
   * @brief Get the Affine object
   *
   * @param yaw yaw angle
   * @param xyz translation coordinates
   * @return Eigen::Affine3d
   */
  Eigen::Affine3d GetAffine(const double yaw, const Eigen::Vector3d& xyz);

  /**
   * @brief Get the coordinates of the eight corners
   *
   * @param rot rotation angle of object
   * @param xyz position of object
   * @return Eigen::Matrix<double, 3, 8>
   */
  Eigen::Matrix<double, 3, 8> GetCorners(double rot, const Eigen::Vector3d& xyz);

  /**
   * @brief Get the coordinate of left-bottom corner
   *
   * @param bbox3d coordinates of eight corners of one object
   * @return Eigen::Vector3d
   */
  Eigen::Vector3d GetLeftBottom(const Eigen::Matrix<double, 3, 8>& bbox3d);

  /**
   * @brief Get coordinate of right-up corner
   *
   * @param bbox3d coordinates of eight corners of one object
   * @return Eigen::Vector3d
   */
  Eigen::Vector3d GetRightUp(const Eigen::Matrix<double, 3, 8>& bbox3d);

  /**
   * @brief init global position from sensor position
   *
   * @param trans tranformation matrix
   */
  void Sensor2Global(const Eigen::Affine3d& trans);

  /**
   * @brief init ego position from global position
   *
   * @param trans tranformation matrix
   */
  void Global2Ego(const Eigen::Affine3d& trans);

  /**
   * @brief Get volumn of the object
   *
   * @return double
   */
  double GetVolumn();

  double det_score;
  uint64_t frame_id;
  int64_t track_id = -1;
  int age = 0;
  ObjectType det_class;
  cv::Scalar show_color;
  bool is_predict = false;
  int64_t asso_rv_id = -1; // tmp，关联的rv目标id

  cv::Rect_<double> rect;
  cv::Rect_<double> rect_tracked;

  Eigen::Vector3d velocity = Eigen::Vector3d(0, 0, 0);
  Eigen::Vector3d velocity_stddev = Eigen::Vector3d(0, 0, 0);
  Eigen::Vector3d acceleration = Eigen::Vector3d(0, 0, 0);
  double velocity_heading;

  Eigen::Vector3d lwh;
  Eigen::Vector3d lwh_tracked;

  // sensor coordinate
  Eigen::Vector3d sensor_xyz;
  Eigen::Vector3d sensor_xyz_stddev;
  Eigen::Vector3d sensor_xyz_tracked;
  double sensor_rot;
  double sensor_rot_stddev;
  double sensor_rot_tracked;
  Eigen::Quaterniond sensor_orientation = Eigen::Quaterniond(1, 0, 0, 0);
  // Eigen::Matrix<double, 3, 8> sensor_bbox3d;
  Eigen::MatrixXd sensor_bbox3d;

  // global coordinate
  Eigen::Vector3d global_xyz;
  Eigen::Vector3d global_xyz_tracked;
  double global_rot;
  double global_rot_tracked;
  Eigen::Quaterniond global_orientation = Eigen::Quaterniond(1, 0, 0, 0);
  Eigen::Matrix<double, 3, 8> global_bbox3d;

  Eigen::Vector3d global_velocity = Eigen::Vector3d(0, 0, 0);
  double global_velocity_heading;


  // ego coordinate
  Eigen::Vector3d ego_xyz;
  double ego_rot;
  Eigen::Quaterniond ego_orientation = Eigen::Quaterniond(1, 0, 0, 0);
};

using ObjectPtr = std::shared_ptr<Object>;
using ObjectConstPtr = std::shared_ptr<const Object>;
#endif // DEVASTATOR_PERCEPTION_FUSION_BASE_OBJECT_H