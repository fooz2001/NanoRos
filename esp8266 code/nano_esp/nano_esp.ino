//Written By M.fayez al masri

#include <ESP8266WiFi.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>


double vl,vr;
const double R = 0.04185;
const double L = 0.0726;
const double O = 0.1315;

String data;
char c;

double X,Y,PHI;

const char* ssid     = "NANO";
const char* password = "11223344";
// Set the rosserial socket server IP address
IPAddress server(192,168,4,4);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

ros::NodeHandle nh;



void messageCb(const geometry_msgs::Twist& msg){
  
  vl = msg.linear.x + (msg.angular.z * L/2);
  vr = msg.linear.x - (msg.angular.z * L/2);
  
//Serial.println(vl);
//Serial.println(vr);
  vel(vl, vr);
}

void commandsCB(const std_msgs::String& msg)
{
  Serial.println(msg.data);
}

ros::Subscriber<geometry_msgs::Twist> sub("vel", messageCb );
ros::Subscriber<std_msgs::String> sub1("commands", commandsCB );

geometry_msgs::Pose2D pos;
std_msgs::Float32 batt;

ros::Publisher pose("pose", &pos);
ros::Publisher volt("batt_volt", &batt);

void setup()
{
  // Use ESP8266 serial to monitor the process
  Serial.begin(115200);

  // Connect the ESP8266 the the wifi AP
  WiFi.softAP(ssid, password);

  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  // Start to be polite
  nh.advertise(pose);
  nh.advertise(volt);

  nh.subscribe(sub);
  nh.subscribe(sub1);
}

void loop()
{ //connect loop
  if (not nh.connected()) {
    Serial.println("Not Connected");
    delay(1000);
  }
  //resive serial data
  while (Serial.available() > 0)
    {
      c=Serial.read();
      if(c=='\n'){break;}
      else{data+=c;}
    }
    if(c=='\n'){
      //Serial.println(data);
      get_pose(data);
      get_volt(data);
      c=0;
      data="";
    }
  
  nh.spinOnce();
  delay(1);
}
