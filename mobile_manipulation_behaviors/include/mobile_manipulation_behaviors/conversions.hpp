#include "geometry_msgs/msg/pose.hpp"
#include "tf2/LinearMath/Quaternion.h"

namespace BT
{
using Pose = geometry_msgs::msg::Pose;


template<>
[[nodiscard]] Pose convertFromString<Pose>(StringView str);

template<>
[[nodiscard]] std::string toStr<Pose>(const Pose & direction);

template<>
Pose convertFromString<Pose>(StringView str)
{
  std::vector<double> pose_parts = convertFromString<std::vector<double>>(str);
  Pose pose;

  if (pose_parts.size() == 4) {
    // Format: x, y, yaw (converts yaw to quaternion with roll=0, pitch=0)
    pose.position.x = pose_parts[0];
    pose.position.y = pose_parts[1];
    pose.position.z = pose_parts[2];

    // Convert yaw to quaternion using tf2
    tf2::Quaternion quat;
    quat.setRPY(0.0, 0.0, pose_parts[3]);  // roll=0, pitch=0, yaw=pose_parts[2]

    pose.orientation.x = quat.x();
    pose.orientation.y = quat.y();
    pose.orientation.z = quat.z();
    pose.orientation.w = quat.w();
  } else if (pose_parts.size() == 7) {
    // Format: x, y, z, qx, qy, qz, qw (direct quaternion)
    pose.position.x = pose_parts[0];
    pose.position.y = pose_parts[1];
    pose.position.z = pose_parts[2];
    pose.orientation.x = pose_parts[3];
    pose.orientation.y = pose_parts[4];
    pose.orientation.z = pose_parts[5];
    pose.orientation.w = pose_parts[6];
  } else {
    throw RuntimeError(
            std::string("Pose requires either 3 values (x, y, yaw) or 7 values (x, y, z, qx, qy, qz, qw). Got: ") +
            static_cast<std::string>(str));
  }

  return pose;
}

template<>
std::string toStr<Pose>(const Pose & pose)
{
  std::stringstream ss;
  ss << pose.position.x << ";" << pose.position.y << ";" << pose.position.z << ";"
     << pose.orientation.x << ";" << pose.orientation.y << ";" << pose.orientation.z
     << ";" << pose.orientation.w;
  return ss.str();
}

std::ostream & operator<<(std::ostream & os, const Pose & pose)
{
  os << toStr(pose);
  return os;
}
}