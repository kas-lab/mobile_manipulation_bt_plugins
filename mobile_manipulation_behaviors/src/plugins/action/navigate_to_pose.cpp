#include "behaviortree_ros2/bt_action_node.hpp"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "behaviortree_ros2/plugins.hpp"
#include "mobile_manipulation_behaviors/conversions.hpp"

using NavigateToPose = nav2_msgs::action::NavigateToPose;
using PoseStamped = geometry_msgs::msg::PoseStamped;
using Pose = geometry_msgs::msg::Pose;

namespace BT
{

class NavigateToPoseAction : public RosActionNode<NavigateToPose>
{
public:
  //Name for the pose input port
  static constexpr const char * POSE = "pose";

  NavigateToPoseAction(
    const std::string & name,
    const NodeConfig & conf,
    const RosNodeParams & params)
  : RosActionNode<NavigateToPose>(name, conf, params)
  {
    RCLCPP_INFO(logger(), "NavigateToPose BT ActionClient Constructed");

  }

  static PortsList providedPorts()
  {
    PortsList base_ports = RosActionNode::providedPorts();
    PortsList child_ports = {
      InputPort<Pose>(POSE)
    };
    child_ports.merge(base_ports);
    return child_ports;
  }

  bool setGoal(RosActionNode::Goal & goal) override
  {
    // #goal definition
    // geometry_msgs/PoseStamped pose
    // string behavior_tree

    goal.behavior_tree = "";
    getInput(POSE, pose_to_navigate_to);

    PoseStamped stamped_pose;
    stamped_pose.header.frame_id = "map";

    stamped_pose.header.stamp = now();
    stamped_pose.pose = pose_to_navigate_to;

    goal.pose = stamped_pose;

    return true;
  }

  // Callback executed when the reply is received.
  // Based on the reply you may decide to return SUCCESS or FAILURE.
  NodeStatus onResultReceived(const WrappedResult & wr) override
  {
    // #result definition
    // std_msgs/Empty result
    //Unfortunately, NavigateToPose in Nav2 humble provides no actual result indicating you reached the pose or not..

    return NodeStatus::SUCCESS;
  }

  virtual NodeStatus onFailure(ActionNodeErrorCode error) override
  {
    RCLCPP_ERROR(logger(), "Navigate to Pose Error: %d", error);
    return NodeStatus::FAILURE;
  }

  NodeStatus onFeedback(const std::shared_ptr<const Feedback> feedback)
  {
    // #feedback definition
    // geometry_msgs/PoseStamped current_pose
    // builtin_interfaces/Duration navigation_time
    // builtin_interfaces/Duration estimated_time_remaining
    // int16 number_of_recoveries
    // float32 distance_remaining
    //TODO: Pipe feedback into Blackboard
    return NodeStatus::RUNNING;
  }

private:
  Pose pose_to_navigate_to;
};

BT_REGISTER_ROS_NODES(factory, params)
{
  RosNodeParams aug_params;
  aug_params.nh = params.nh;
  aug_params.server_timeout = std::chrono::milliseconds(40000);   //Nav2 can take a while to respond, especialy in a container.
  //TODO: options.use_global_arguments(false) need to fix this for plguins somehow. also applies to client name
  factory.registerNodeType<NavigateToPoseAction>("navigateToPose", aug_params);
}

}
