#include <vector>
#include "geometry_msgs/msg/pose.hpp"
#include "behaviortree_cpp/decorator_node.h"


namespace BT
{

using Pose = geometry_msgs::msg::Pose;

class NextPose : public BT::DecoratorNode
{
  public:
    NextPose(
      const std::string & name,
      const NodeConfig & conf)
    : BT::DecoratorNode(name, conf)
    {
    }

    static PortsList providedPorts()
    {
      return {
        InputPort<std::vector<Pose>>("poses"),
        OutputPort<Pose>("pose_i")
      };
    }

    BT::NodeStatus tick()
    {
      setStatus(BT::NodeStatus::RUNNING);
      auto poses_res = getInput("poses", poses_to_go_to_);
      if(!poses_res)
      {
        return BT::NodeStatus::FAILURE;
      }

      setOutput("pose_i",poses_to_go_to_[index_]);
      const BT::NodeStatus child_state = child_node_->executeTick();

      switch (child_state) {
        case BT::NodeStatus::SKIPPED:
        case BT::NodeStatus::RUNNING:
          return child_state;

        case BT::NodeStatus::FAILURE:
        case BT::NodeStatus::SUCCESS:
          index_++;
          if(index_ > poses_to_go_to_.size())
          {
            return child_state;
          }
          else
          {
            return BT::NodeStatus::RUNNING;
          }

        default:
          index_++;
          return BT::NodeStatus::FAILURE;
      }


      return BT::NodeStatus::FAILURE;
    }

  private:
    int index_ = 0;
    std::vector<Pose> poses_to_go_to_;
};

}  // namespace BT

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<BT::NextPose>("NextPose");
}
