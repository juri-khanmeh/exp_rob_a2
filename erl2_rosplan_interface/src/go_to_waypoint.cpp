#include "erl2_rosplan_interface/go_to_waypoint.h"
#include <unistd.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <motion_plan/PlanningAction.h>

namespace KCL_rosplan {

	MyActionInterface::MyActionInterface(ros::NodeHandle &nh) {
	// here the initialization
	}
	
	bool MyActionInterface::concreteCallback(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg) {
		// here the implementation of the action
		std::cout << "Going from " << msg->parameters[1].value << " to " << msg->parameters[2].value << std::endl;
		
		//sleep(5);
		actionlib::SimpleActionClient<motion_plan::PlanningAction> ac("reaching_goal", true);
		motion_plan::PlanningGoal goal;
		ac.waitForServer();
		float pos=2.6;
		if(msg->parameters[2].value == "wp1"){
			goal.target_pose.pose.position.x = -pos;
			goal.target_pose.pose.position.y = 0.0;
			goal.target_pose.pose.orientation.w = 0.0;
		}
		else if (msg->parameters[2].value == "wp2"){
			goal.target_pose.pose.position.x = pos;
			goal.target_pose.pose.position.y = 0.0;
			goal.target_pose.pose.orientation.w = 0.0;
		}
		else if (msg->parameters[2].value == "w3"){
			goal.target_pose.pose.position.x = 0.0;
			goal.target_pose.pose.position.y = -pos;
			goal.target_pose.pose.orientation.w = 0.0;
		}
		else if (msg->parameters[2].value == "wp4"){
			goal.target_pose.pose.position.x = 0.0;
			goal.target_pose.pose.position.y = pos;
			goal.target_pose.pose.orientation.w = 0.0;
		}
		else if (msg->parameters[2].value == "wp0"){
			goal.target_pose.pose.position.x = 0.0;
			goal.target_pose.pose.position.y = 0.0;
			goal.target_pose.pose.orientation.w = 0.0;
		}

		ac.sendGoal(goal);
		ac.waitForResult();
		ROS_INFO("Action (%s) performed: completed!", msg->name.c_str());
		return true;
	}
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "gotowaypoint_rosplan_action", ros::init_options::AnonymousName);
	ros::NodeHandle nh("~");
	KCL_rosplan::MyActionInterface my_aci(nh);
	my_aci.runActionInterface();
	return 0;
}
