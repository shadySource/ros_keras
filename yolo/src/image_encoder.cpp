/***********************************
Created by:
    https://github.com/Appleman8977
    https://github.com/stoplime
************************************/
#include "ros/ros.h"
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>

class ImageEncoder{
private:
	ros::NodeHandle n;
	cv::VideoCapture cap;
	std::string topic_name;
	// ros::Rate loop_rate;
	cv::Mat image;

	std::vector<uchar> data;
	std_msgs::UInt8MultiArray dat;

	ros::Publisher image_pub;

public:
	ImageEncoder(int video_channel, std::string pub_topic)
	: cap(video_channel)//, loop_rate(mili_rate)
	{
		image_pub = n.advertise<std_msgs::UInt8MultiArray>(pub_topic, 1000);

		dat.layout.dim.push_back(std_msgs::MultiArrayDimension());
		dat.layout.dim[0].label = "img_data";
		dat.layout.dim[0].stride = 7;
		dat.layout.data_offset = 0;
	}

	void runOnce(){
		cap >> image;

		if (!image.empty()){
			cv::imencode(".jpg", image, data);
			dat.data = data;
			image_pub.publish(dat);
			ros::spinOnce();
			data.clear();
			image.release();
		}
		else{
			ROS_WARN("Camera image empty");
		}
		// loop_rate.sleep();
	}

};

using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "SensorMain");
	ImageEncoder camera0(0, "/sensors/camera0/jpegbuffer");

	ros::Rate loop_rate(120);
	while (ros::ok())
	{
		camera0.runOnce();

		ros::spinOnce();
		loop_rate.sleep();
	}
    return 0;
}
