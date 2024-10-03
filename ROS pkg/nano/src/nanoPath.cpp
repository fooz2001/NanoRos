#include <ros/ros.h>

#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/Pose2D.h>

nav_msgs::Path path;
geometry_msgs::PoseStamped pose;
tf2::Quaternion q;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "path");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<nav_msgs::Path>("nanoPath", 10);


  auto poseCb = [&](const geometry_msgs::Pose2D::ConstPtr& msg) {
    if((pose.pose.position.x != msg ->x)and(pose.pose.position.y != msg ->y)){
      pose.header.frame_id = "nanoPose";
      pose.header.stamp = ros::Time::now();

      pose.pose.position.x = msg -> x;
      pose.pose.position.y = msg -> y;

      q.setRPY(0, 0, msg -> theta);
      q.normalize();

      pose.pose.orientation.w = q[0];
      pose.pose.orientation.x = q[1];
      pose.pose.orientation.y = q[2];
      pose.pose.orientation.z = q[3];

      path.header.frame_id = "nanoPath";
      path.header.stamp = ros::Time::now();

      path.poses.push_back(pose);
      pub.publish(path);
    }
  };

  auto sub = n.subscribe<geometry_msgs::Pose2D>("pose", 10, poseCb);

  ros::spin();  
}