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

struct hint{
	std_msgs::String type;
	std_msgs::String info;	
};

struct data_table{
	int counter; // counter of hints for same ID
	struct hint hints[5];
};

data_table* Data = new data_table[6]; // hint list for 6 IDs


erl2::ErlOracle oracle_msg; 
bool new_hint;

bool check_consistence(int ID){
	bool who = false; bool what = false; bool where = false;
	for(int j=0; j<5; j++){
 		if(Data[ID].hints[j].type.data =="who"){
 			who = true; break;
 		}
 	}
 	if(who){
 		for(int j=0; j<5; j++){
 			if(Data[ID].hints[j].type.data =="what"){
 				what = true; break;
 			}
 		}
 		if(what){
 			for(int j=0; j<5; j++){
 				if(Data[ID].hints[j].type.data =="where"){
 					return true;
 				}	
 			}
 			return false;
 		}
 		else return false;
 	}
 	else return false; 
}

namespace KCL_rosplan {

	MyActionInterface::MyActionInterface(ros::NodeHandle &nh) {
	// here the initialization
	}
	
	bool MyActionInterface::concreteCallback(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg) {
		// here the implementation of the action
		ROS_INFO("Updating Ontology");
		//sleep(1);
		if(new_hint){
			//return true; 
			int i = oracle_msg.ID;
 			int k = Data[i].counter;
 			Data[i].hints[k].type.data = oracle_msg.key;
 			Data[i].hints[k].info.data = oracle_msg.value;
 			Data[i].counter = k + 1;
 			ROS_INFO("Hint ID[%d] number[%d] ", i, k);
 			//ROS_INFO("ID = %d , key = %s, value = %s", oracle_msg.ID, oracle_msg.key, oracle_msg.value);
 			
  			new_hint = false;
 			bool consistent;
 			if(k>=2){
 				consistent = check_consistence(i); 
 				if(consistent){ 
 					ros::param::set("/consistent_ID", i);
 					ROS_INFO("Hypothesis with ID = %d is consistent", oracle_msg.ID);
 					ROS_INFO("Action (%s) performed: completed!", msg->name.c_str());
 					return true; 
 				}
 				else {
 					ROS_INFO("Hypothesis with ID = %d is INconsistent", oracle_msg.ID);
 					return false;
 				}
 			}
 			else {
 				ROS_INFO("Incomplete Hypothesis... ");
 				return false;
 			}
		}
		else {
			ROS_INFO("No hint... ");
 			return false;
		}

	}
}

void oracleCallback(const erl2::ErlOracle::ConstPtr& msg){
	oracle_msg = *msg;
	ROS_INFO("new_hint = true");
	new_hint = true;
}


int main(int argc, char **argv) {
	ros::init(argc, argv, "ontology_action", ros::init_options::AnonymousName);
	ros::NodeHandle nh("~");
	ros::Subscriber sub = nh.subscribe("/oracle_hint", 10, oracleCallback);
	KCL_rosplan::MyActionInterface my_aci(nh);
	my_aci.runActionInterface();
	
	//ros::spin();
	return 0;	
}
