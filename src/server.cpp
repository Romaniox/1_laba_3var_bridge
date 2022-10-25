#include "ros/ros.h"
#include "glass_stepping_stones/Move.h"
#include <std_msgs/Char.h>

const std::string CORRECT_PATH = "lrlrllllrlrrrlrll";
std::string g_passed_path;
uint8_t g_step = 0;

ros::Publisher publisher;

bool getMove(glass_stepping_stones::Move::Request & req,
  glass_stepping_stones::Move::Response & res) 
  {
  if (req.option != 'l' && req.option != 'r') 
  {
    return false;
  }

  if (req.option == CORRECT_PATH[g_step]) 
  {
    g_passed_path.insert(g_step, 1, req.option);

    if (g_passed_path == CORRECT_PATH) 
    {
      ROS_INFO_STREAM("congratulations, you won");
    } 
    else 
    {
      res.result = "move next";
      ROS_INFO_STREAM(g_passed_path);
    }

    std_msgs::Char chosen_option;
    chosen_option.data = req.option;

    publisher.publish(chosen_option);

    g_step++;
  } 
  else 
  {
    res.result = "dead";
    ros::shutdown();
  }
  return true;
}

int main(int argc, char ** argv) 
{
  ros::init(argc, argv, "bridge_server");
  ros::NodeHandle n;

  publisher = n.advertise < std_msgs::Char > ("topic_out", 10);
  ros::ServiceServer service = n.advertiseService("get_move", getMove);
  ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}