#include "erl2_rosplan_interface/go_to_waypoint.h"
#include <unistd.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <motion_plan/PlanningAction.h>

#include <ros/ros.h>

// MoveIt
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

namespace KCL_rosplan {

	MyActionInterface::MyActionInterface(ros::NodeHandle &nh) {
	// here the initialization
	}
	
	bool MyActionInterface::concreteCallback(const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg) {
		// here the implementation of the action
		std::cout << "Moving arm from Up to Down" << std::endl;
		
		//sleep(5);
		//actionlib::SimpleActionClient<motion_plan::PlanningAction> ac("reaching_goal", true);
		//motion_plan::PlanningGoal goal;
		//ac.waitForServer();
		robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
  		const moveit::core::RobotModelPtr& kinematic_model = robot_model_loader.getModel();
  		ROS_INFO("Model frame: %s", kinematic_model->getModelFrame().c_str());

  		moveit::core::RobotStatePtr kinematic_state(new moveit::core::RobotState(kinematic_model));
  		kinematic_state->setToDefaultValues();
  		const moveit::core::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("arm");
  		moveit::planning_interface::MoveGroupInterface group("arm");
  		const std::vector<std::string>& joint_names = joint_model_group->getVariableNames();

  
  		//kinematic_state->setToRandomPositions(joint_model_group);
  		//const Eigen::Isometry3d& pose1 = kinematic_state->getGlobalLinkTransform("arm_link_04");
  		float xx= 0.5;
  		geometry_msgs::Pose pose1;
	
  		pose1.orientation.w = 0.00;
  		pose1.orientation.x = -0.00;
  		pose1.orientation.y = 0.00;
  		pose1.orientation.z = -0.00;
  		pose1.position.x =  0.00;
  		pose1.position.y =  xx;
  		pose1.position.z =  0.75;
  
  
  		// Inverse Kinematics
  		// ^^^^^^^^^^^^^^^^^^
  		// We can now solve inverse kinematics (IK) for the Panda robot.
  		// To solve IK, we will need the following:
  		//
  		//  * The desired pose of the end-effector 
  		//  * The timeout: 0.1 s
  
  		group.setStartStateToCurrentState();
  		group.setApproximateJointValueTarget(pose1, "arm_link_04");
  		std::vector<double> joint_values;
  		double timeout = 0.1;
  		bool found_ik = kinematic_state->setFromIK(joint_model_group, pose1, timeout);

  		// Now, we can print out the IK solution (if found):
  		if (found_ik)
  		{
  		  kinematic_state->copyJointGroupPositions(joint_model_group, joint_values);
    		for (std::size_t i = 0; i < joint_names.size(); ++i)
    		{
      		ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_values[i]);
    		}
  		}
  		else
  		{
    		ROS_INFO("Did not find IK solution");
  		}
  
  
  		group.setJointValueTarget(joint_values);
  		group.setStartStateToCurrentState();
  		group.setGoalOrientationTolerance(0.01);
  		group.setGoalPositionTolerance(0.01);

  		// Plan and execute
  		moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  		group.plan(my_plan); 
  		group.execute(my_plan);
  
  		std::cout << "Position 1 -> IK + setJointValue" << std::endl;
  		sleep(3.0);
		//float pos=2.6;
		//if(msg->parameters[2].value == "wp1"){
		//	goal.target_pose.pose.position.x = -pos;
		//	goal.target_pose.pose.position.y = 0.0;
		//	goal.target_pose.pose.orientation.w = 0.0;
		//}

		//ac.sendGoal(goal);
		//ac.waitForResult();
		ROS_INFO("Action (%s) performed: completed!", msg->name.c_str());
		return true;
	}
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "lower_ee_action", ros::init_options::AnonymousName);
	ros::NodeHandle nh("~");
	KCL_rosplan::MyActionInterface my_aci(nh);
	my_aci.runActionInterface();
	return 0;
	
	//ros::init(argc, argv, "robot_model_and_robot_state_tutorial");
  	//ros::AsyncSpinner spinner(1);
  	//spinner.start();
  
  	// We start by instantiating a
  	// `RobotModelLoader`_
  	// object, which will look up
  	// the robot description on the ROS parameter server and construct a
  	// :moveit_core:`RobotModel` for us to use.
  	//
  	// .. _RobotModelLoader:
  	//     http://docs.ros.org/noetic/api/moveit_ros_planning/html/classrobot__model__loader_1_1RobotModelLoader.html
 
  	//ros::shutdown();
  	//return 0;

}