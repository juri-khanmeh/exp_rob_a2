#include "erl2_rosplan_interface/my_action.h"
#include <unistd.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <motion_plan/PlanningAction.h>

#include <erl2/ErlOracle.h>
#include <erl2/Oracle.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <ros/ros.h>

#include "std_msgs/String.h"

bool action_call = false;
erl2::Oracle srv;
int correct_ID;

namespace KCL_rosplan {

	MyActionInterface::MyActionInterface(ros::NodeHandle &nh) {
	// here the initialization
	}
	
	bool MyActionInterface::concreteCallback(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg) {
		// here the implementation of the action
		ROS_INFO("Checking Solution");
		action_call = true;
		sleep(2);
		ROS_INFO("Correct ID= %d", correct_ID);
		int consistant_ID;
		ros::param::get("/consistent_ID", consistant_ID);
		if (consistant_ID == correct_ID){
			ROS_INFO("Hypothesis with ID %d is CORRECT!", correct_ID );
			ROS_INFO("Action (%s) performed: completed!", msg->name.c_str());
			action_call = false;
			return true;
		}
		else{
			ROS_INFO("Hypothesis with ID %d is INCORRECT!", correct_ID );
			action_call = false;
			return false;
		}

	}
}


int main(int argc, char **argv) {
	ros::init(argc, argv, "check_solution_action", ros::init_options::AnonymousName);
	ros::NodeHandle nh("~");
	KCL_rosplan::MyActionInterface my_aci(nh);
	my_aci.runActionInterface();
	ros::ServiceClient client = nh.serviceClient<erl2::Oracle>("/oracle_solution");
	
	while (ros::ok()){
		if(action_call){
			erl2::Oracle srv;
			if (client.call(srv))
			{
				correct_ID = srv.response.ID;	
			}
		}	
	}
	
	//ros::spin();
	
	return 0;
	
}
