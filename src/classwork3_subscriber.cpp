#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "classwork3_pkg/Sinusoidal_msg.h"
#include "boost/thread.hpp"

using namespace std;

class ROS_SUB {
	public:
		ROS_SUB();
		void topic_cb( classwork3_pkg::Sinusoidal_msg sin_msg);
        void filter();   
		ros::NodeHandle get_NodeHandle();
	private:
		ros::NodeHandle _nh;
		ros::Subscriber _topic_sub;	   //per leggere il valore della sinusoide
		ros::Publisher _topic_pub;     //per pubblicare il valore filtrato
        float _value;      //salva il valore della sinusoide
};

ros::NodeHandle ROS_SUB::get_NodeHandle(){
	return _nh;
}

ROS_SUB::ROS_SUB() {
	_value = 0;
	_topic_sub = _nh.subscribe("/sinusoidal", 1, &ROS_SUB::topic_cb, this);   //leggi dal topic sinusoidal
	_topic_pub = _nh.advertise<std_msgs::Float32>("/filtered", 1);  //scrivi sul topic filtered

    boost::thread filter_thread(&ROS_SUB::filter,this);  //thread che implementa il filtraggio
}


void ROS_SUB::topic_cb( classwork3_pkg::Sinusoidal_msg sin_msg) {
//	cout << "cb amplitude: " << sin_msg.amplitude << endl;		//DEBUG
//	cout << "cb period: " << sin_msg.period << endl;
//	cout << "cb value: " << sin_msg.value << endl;
	_value = sin_msg.value;		//aggiorna il value
}


void ROS_SUB::filter(){		
	ros::Rate rate(100);  //100 hz
	float old_filtered_value = 0.0;
	float filtered_value = 0.0;
	std_msgs::Float32 filter_msg;
	filter_msg.data = 0.0;
	while(true){
	filtered_value = 0.99*old_filtered_value + _value*0.00995;  	//frequenza di taglio a 2 PI
	old_filtered_value = filtered_value;
//	cout << "FILTRO:"<< filtered_value.data << endl;  //DEBUG
	filter_msg.data = filtered_value;
	_topic_pub.publish(filter_msg);		//pubblica il valore filtrato
	rate.sleep();
	}
}

int main( int argc, char** argv ) {
	ros::init(argc, argv, "classwork3_subscriber");
	ROS_SUB rs;
	ros::spin();
	return 0;
}


