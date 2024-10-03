#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>

geometry_msgs::PoseStamped pose;
geometry_msgs::PoseStamped goal;
nav_msgs::Path path;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "my_robot_controller");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<nav_msgs::Path>("rrtPath", 1);

  path.header.frame_id = "rrtPath";

  auto poseCb = [&](const geometry_msgs::PoseStamped::ConstPtr& msg) {
    pose.header = msg -> header;
    pose.pose = msg -> pose;

    path.header.stamp = ros::Time::now();

    path.poses.push_back(pose);
    pub.publish(path);

    if((pose.pose.position.x == goal.pose.position.x)and(pose.pose.position.y == goal.pose.position.y)){
      path.poses.clear();
      ROS_INFO("clear");
    }

  };

  auto sub = n.subscribe<geometry_msgs::PoseStamped>("rrtPoses", 1, poseCb);
  
  auto goalCb = [&](const geometry_msgs::PoseStamped::ConstPtr& msg) {
    goal.pose.position.x = msg ->pose.position.x;
    goal.pose.position.y = msg ->pose.position.y;

    ROS_INFO_STREAM(goal.pose.position.x);
    ROS_INFO_STREAM(goal.pose.position.y);

  };

  auto sub1 = n.subscribe<geometry_msgs::PoseStamped>("move_base_simple/goal", 1, goalCb);

  ros::spin();
}