#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/thread.hpp>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Odometry.h>
#include <typeinfo>

class multiThreadListener
{
public:
    void chatterCallback1(const nav_msgs::Odometry::ConstPtr &input_msg, std::string topicName);
    void chatterCallback2(const sensor_msgs::PointCloud2ConstPtr& input_msg, std::string topicName);
    multiThreadListener();

private:
    ros::NodeHandle n;
    std::string name1;
    std::string name2;
    ros::Subscriber sub1;
    ros::Subscriber sub2;
};

multiThreadListener::multiThreadListener()
{
    name1 = "/navsat/odom";
    name2 = "/radar_cloud";

    sub1 = n.subscribe<nav_msgs::Odometry>("/navsat/odom", 10,
                                           boost::bind(&multiThreadListener::chatterCallback1, this, _1, name1));
    sub2 = n.subscribe<sensor_msgs::PointCloud2>("/radar_cloud", 10,
                                                 boost::bind(&multiThreadListener::chatterCallback2, this,_1, name2));
}

void multiThreadListener::chatterCallback1(const nav_msgs::Odometry::ConstPtr &input_msg, std::string topicName)
{
    ROS_INFO("I heard: [%s]", topicName);
    ros::Rate loop_rate(0.5);//block chatterCallback1()
    loop_rate.sleep();
}


void multiThreadListener::chatterCallback2(const sensor_msgs::PointCloud2ConstPtr& input_msg, std::string topicName)
{
    ROS_INFO("I heard: [%s]", topicName);
    ros::Rate loop_rate(0.5);//block chatterCallback2()
    loop_rate.sleep();
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "multi_sub");

    multiThreadListener listener_obj;
    ros::MultiThreadedSpinner s(2);
    ros::spin(s);

    return 0;
}