#include "ros/ros.h" 
#include "classwork3_pkg/Sinusoidal_msg.h"  //nome del messaggio custom

int main(int argc, char **argv) {

	ros::init(argc, argv,"classwork3_publisher");
	ros::NodeHandle nh;
	ros::Publisher topic_pub = nh.advertise<classwork3_pkg::Sinusoidal_msg>("/sinusoidal", 1);  //nome del topic dove pubblicare la sinusoide

	ros::Rate rate(100);  //100 hz

    //Define the custom datatype
    classwork3_pkg::Sinusoidal_msg sin_msg;
    //Fill the name part

	sin_msg.amplitude = 1.0;
    sin_msg.period = 2*M_PI/10.0;   //una decade a destra della frequenza di taglio del filtro
    sin_msg.value = 0; 				//valore iniziale
 
	double t = 0.0;  
	double dt = 0.01;     //100 hz

	while ( ros::ok() ) {
		topic_pub.publish(sin_msg); //pubblica poi aggiorna il valore
		sin_msg.value = sin_msg.amplitude*sin(2*M_PI/sin_msg.period*t);
		t = t+dt;
		rate.sleep();
	}
	
	return 0;
}