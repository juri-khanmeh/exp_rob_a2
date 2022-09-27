(define (domain cluedo)

(:requirements :strips :typing :fluents :disjunctive-preconditions :durative-actions)

(:types
	waypoint 
	robot
)

(:predicates
	(robot_at ?r - robot ?wp - waypoint)
	(arm_up ?r - robot)
	(arm_down ?r - robot)
	(has_consistent_hypo ?r - robot)
	(has_correct_hypo ?r - robot)
	(hint_exist ?wp - waypoint)
	(is_center ?wp - waypoint)
	(isnot_center ?wp - waypoint)
	(got_hint ?r - robot)
)

;; Move to (not center) waypoint, avoiding terrain
(:durative-action goto_waypoint
	:parameters (?r - robot ?from ?to - waypoint)
	:duration ( = ?duration 60)
	:condition (and
		(at start (robot_at ?r ?from))
		(over all (isnot_center ?from))
		(over all (isnot_center ?to))
		(over all (hint_exist ?to))
		(over all (arm_down ?r)))
	:effect (and
		(at start (not (robot_at ?r ?from)))
		(at end (robot_at ?r ?to))
		(at end (hint_exist ?from)))
)

;; Move from center(0,0)
(:durative-action fromcenter_waypoint
	:parameters (?r - robot ?from ?to - waypoint)
	:duration ( = ?duration 60)
	:condition (and
		(at start (robot_at ?r ?from))
		(over all (arm_down ?r))
		(over all (is_center ?from))
		(over all (isnot_center ?to)))
	:effect (and
		(at start (not (robot_at ?r ?from)))
		(at end (robot_at ?r ?to)))
)

;; Move to center(0,0)
(:durative-action waypoint_tocenter
	:parameters (?r - robot ?from ?to - waypoint)
	:duration ( = ?duration 60)
	:condition (and
		(at start (robot_at ?r ?from))
		(over all (arm_down ?r))
		(over all (isnot_center ?from))
		(over all (is_center ?to))
		(over all (has_consistent_hypo ?r)))
	:effect (and
		(at start (not (robot_at ?r ?from)))
		(at end (robot_at ?r ?to))
		(at end (hint_exist ?from)))
)

;; Raise up the arm of the robot
(:durative-action raise_arm
	:parameters (?r - robot ?wp - waypoint)
	:duration ( = ?duration 30)
	:condition (and
		(at start (arm_down ?r))
		(at start (hint_exist ?wp))
		(over all(robot_at ?r ?wp)))
	:effect (and 
		(at end (got_hint ?r))
		(at end (not (hint_exist ?wp)))
		(at end (arm_up ?r))
		(at start (not (arm_down ?r))))
)

;; Lower the arm of the robot
(:durative-action lower_arm
	:parameters (?r - robot)
	:duration ( = ?duration 30)
	:condition (at start (arm_up ?r))
	:effect (and 
		(at end (arm_down ?r))
		(at start (not (arm_up ?r))))
)

;; Update Cluedo Ontology
(:durative-action update_ontology
	:parameters (?r - robot)
	:duration ( = ?duration 10)
	:condition (at start(got_hint ?r))
	:effect (and 
		(at end (has_consistent_hypo ?r))
		(at start (not (got_hint ?r))))
)


;; Check the consistant hypothesis
(:durative-action check_hypothesis
	:parameters (?r - robot ?wp - waypoint)
	:duration ( = ?duration 10)
	:condition (and
		(at start(has_consistent_hypo ?r))
		(over all(robot_at ?r ?wp))
		(over all(is_center ?wp)))
	:effect (and 
		(at start (not (has_consistent_hypo ?r)))
		(at end (has_correct_hypo ?r)))
)

)
