#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>

#include <cmath>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "points_and_lines");
  ros::NodeHandle n;
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("rrtPath", 10);

  ros::Rate r(30);

  geometry_msgs::Point pose;
  geometry_msgs::PoseStamped goal;
  visualization_msgs::Marker points, line_strip;
  visualization_msgs::MarkerArray mArray;

  int count = 0;
  bool nextPath;

  auto poseCb = [&](const geometry_msgs::PoseStamped::ConstPtr& msg) {

    if(nextPath){
      points.action = visualization_msgs::Marker::DELETE;
      line_strip.action = visualization_msgs::Marker::DELETE;
      points.points.clear();
      line_strip.points.clear();
      marker_pub.publish(points);
      marker_pub.publish(line_strip);

      nextPath = false;
    }

    points.header.frame_id = line_strip.header.frame_id = "rrtPath";
    points.header.stamp = line_strip.header.stamp = ros::Time::now();
    points.ns = line_strip.ns = "path";

    points.id = 0;
    line_strip.id = 1;

    points.action = line_strip.action = visualization_msgs::Marker::ADD;
    points.pose.orientation.w = line_strip.pose.orientation.w = 1.0;

    points.type = visualization_msgs::Marker::POINTS;
    line_strip.type = visualization_msgs::Marker::LINE_STRIP;

    // POINTS markers use x and y scale for width/height respectively
    points.scale.x = 0.5;
    points.scale.y = 0.5;

    // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
    line_strip.scale.x = 0.3;

    // Points are green
    points.color.r = 1.0;
    points.color.a = 1.0;

    // Line strip is blue
    line_strip.color.b = 1.0;
    line_strip.color.a = 1.0;
    pose.x = msg ->pose.position.x;
    pose.y = msg ->pose.position.y;
    
    points.points.push_back(pose);
    line_strip.points.push_back(pose);

    if((goal.pose.position.x == pose.x) and (goal.pose.position.y == pose.y)){

        ROS_INFO("goal found");
        marker_pub.publish(points);
        marker_pub.publish(line_strip);
        ROS_INFO_STREAM(count);
        count = 0;

        nextPath = true;
    }
    count++;
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