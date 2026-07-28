#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class Nav2PluginConfigurer : public rclcpp::Node
{
  public:
    Nav2PluginConfigurer(rclcpp::NodeOptions & options)
    : Node("nav2_plugin_configurer", options.automatically_declare_parameters_from_overrides(true))
    {
      // Any publisher to this topic needs to be configured with some QoS defined as reliable and transient local.

      rclcpp::QoS qos(rclcpp::KeepLast(1));
      qos.transient_local().reliable();

      this->get_parameter("controller_topic", controller_topic_name_);
      this->get_parameter("planner_topic", planner_topic_name_);
      this->get_parameter("smoother_topic", smoother_topic_name_);
      this->get_parameter("goal_checker_topic", goal_checker_topic_name_);

      controller_selector_pub_ = this->create_publisher<std_msgs::msg::String>(controller_topic_name_,qos);
      planner_selector_pub_ = this->create_publisher<std_msgs::msg::String>(planner_topic_name_,qos);
      smoother_selector_pub_ = this->create_publisher<std_msgs::msg::String>(smoother_topic_name_,qos);
      goal_checker_selector_pub_ = this->create_publisher<std_msgs::msg::String>(goal_checker_topic_name_,qos);

      timer_ = this->create_wall_timer(
      500ms, std::bind(&Nav2PluginConfigurer::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      this->get_parameter("selected_controller", selected_controller_);
      this->get_parameter("selected_planner", selected_planner_);
      this->get_parameter("selected_smoother", selected_smoother_);
      this->get_parameter("selected_goal_checker", selected_goal_checker_);

      auto controller_message = std_msgs::msg::String();
      auto planner_message = std_msgs::msg::String();
      auto smoother_message = std_msgs::msg::String();
      auto goal_checker_message = std_msgs::msg::String();

      controller_message.data = selected_controller_;
      planner_message.data = selected_planner_;
      smoother_message.data = selected_smoother_;
      goal_checker_message.data = selected_goal_checker_;

      // message.data = "Hello, world! " + std::to_string(count_++);
      controller_selector_pub_->publish(controller_message);
      planner_selector_pub_->publish(planner_message);
      smoother_selector_pub_->publish(smoother_message);
      goal_checker_selector_pub_->publish(goal_checker_message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr controller_selector_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr planner_selector_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr smoother_selector_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr goal_checker_selector_pub_;

    std::string controller_topic_name_;
    std::string planner_topic_name_;
    std::string smoother_topic_name_;
    std::string goal_checker_topic_name_;

    std::string selected_controller_;
    std::string selected_planner_;
    std::string selected_smoother_;
    std::string selected_goal_checker_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto options = rclcpp::NodeOptions();
  rclcpp::spin(std::make_shared<Nav2PluginConfigurer>(options));
  rclcpp::shutdown();
  return 0;
}
