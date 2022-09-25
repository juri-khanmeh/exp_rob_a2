# Experimental Robotics Laboratory / assignment 2

This package contains 3 pakcages for  build a simulation on top of the architecture that we have developed in the first
assignment. The simulation is about Cluedo game in a limitied conditions.
Now, the Robot is a real robot, in a physical environment. 
There are four positions in the environment (x,y,z) that contains hints: if the «cluedo_link» (the name should be exactly this one) of the robot is reasonably close, this will trigger the oracle for the generation of a hint

## Description 
### Packages
#### erl2
This package was the starting point for this assignment.
The package contains:
- a node, which implements the oracle and the visualization of the four «hint» positions. Concerning the implementation of the Oracle consider that:
- there are in total 6 IDs [0&5]
- some of these IDs (randomly chosen) may generate inconsistent hypotheses (e.g. multiple persons, rooms, objects)
- the «trustable» ID is also randomly chosen (among the IDs which do not generate inconsistent hypothese)
- the hints are sent on the topic /oracle_hint as a erl2/ErlOracle.h message
- the node implements also a service (/oracle_solution) which returns the trustable ID (erl2/Oracle.h, with an empty message for the request, and a int32 for the reply)

Modification in the package:
- We modified the main launch file in this package by spwaning a robot model
- We add 'plan.launch' file, in which we launch the the node necessary to run ROSplan
- In 'config' folder, we add some files for controlling the arm of the robot
- In 'urdf' folder, we add a urdf file of our model of robot
- In 'common' folder, we add 'domain_cluedo.pddl' and 'problem_cluedo.pddl' files
- In the 'simulation.cpp' file we changed 'cluedo_link' with 'arm_link_04'
#### erl2_moveit
This package was built when we used MoveIt for robot modelling and control
#### erl2_rosplan_interface
In this package, we defined our action when implementing rosplanning.
Actions:
* 'go_to_waypoint' The action of making the robot move from one point to another
* 'lower_ee' The action of lowering the end-effector of the arm to a certain altitude
* 'raise_ee' The action of raising up the end-effector of the arm to a certain altitude
* 'ontology' The action which is executed when the robot updates his ontology of the game. Updating ontology means to check if the robot has a complete hypothesis. Then check if it is a consistent one.
* 'oracle' The action that check if the consistent hypothesis is correct or not
### ROSPlan
In this assignment we use ROSPlan to plan the behaviour of the robot. 
Acions, domain and a problem were created for this specific case.
#### Domain
* Assumption:
- Every time the robot move to a waypoint, it must raise its arm to high z = 1.25
- The robot cannot move from one point to another if the arm is not low z = 0.75
- When the robot arrives to a waypoint and takes the hint, the hint does not exist anymore
- Once the robot leaves the waypoint, a hint is generated in that waypoint
- The robots can move from on waypoint to another but not to the center
- The robot moves to the center only when it has a consistence hypothesis

* Note:
The pddl file of the domain can be found '/erl2/common/domain_cluedo.pddl'
#### Problem
* Initializaion:
- The robot is at the center
- And all the waypoint have hints
- The robot does not have a consistent hypothesis

* Goal:
The robot gets the correct hypothesis

* Note:
The pddl file can be found '/erl2/common/problem_cluedo.pddl'

## Instruction how to run the code:

1. After moving the 3 packages to the ws and build the packages, open a terminal (ROS sourced) then execute
```
roslaunch erl2 assignment2_2.launch
```
2. After that launch
```
roslaunch erl2 plan.launch
```
3. Open another terminal (ROS sourced) and call the service for the problem generation
```
rosservice call /rosplan_problem_interface/problem_generation_server
```
4. Call the service for the plan generation
```
rosservice call /rosplan_planner_interface/planning_server
```
5. Call the service to parse the plan
```
rosservice call /rosplan_parsing_interface/parse_plan
```
6. Start the action dispatching by calling another service
```
rosservice call /rosplan_plan_dispatcher/dispatch_plan
```
7. When the plan fails and we need to replan we repeat the service call from 3. to 6.


## Working Hypothesis and environment
### System's feature
The system concerns of:
* The system has a simulation environment, a representation for hints and moving robots
* Robot's arm is controlled
* Implementing ROSPlan to manage the actions of the robot
* Executing real actions when dispatching the plan
* Visualization of the execution of the plan

Note:
* The system works in ROS environment.
* ROSPlan must be installed

### System's limitations
After applying our plan we noticed several issues:
1. Since the ontology is updated everytime the robot gets hint, so with our defined domain, we must replan on each visit of room
2. The visited room are only wp1 and wp2
3. Since the hint position are defined relatively to the body frame, if the robot arrives to the waypoint not aligned towards that point the robot might not be able to get the hint

### Possible improvements
* Regarding point 1. and 2. we need to modify our domain and problem in order to make the robot visit all the rooms. Besides, the plan must fail at least when the robot has a complete hypothesis
* Regarding point 3. we need to define the hints' positions as absolute positions with respect to odom frame. In this way even if the robot was not aligning towards the hint, it is able to get the hint. Or, we keep the relative position however we need to make the robot align towards the hint before starting to move the arm to make sure that the robot gets the hint.
* Using ARMOR can be easier to update the ontology of the game


