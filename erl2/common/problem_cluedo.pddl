(define (problem task)
(:domain cluedo)
(:objects
    wp0 wp1 wp2 wp3 wp4 - waypoint
    sherlock - robot
)
(:init
    (robot_at sherlock wp0)
    (is_center wp0)
    (isnot_center wp1)
    (isnot_center wp2)
    (isnot_center wp3)
    (isnot_center wp4)
    (hint_exist wp1)
    (hint_exist wp2)
    (hint_exist wp3)
    (hint_exist wp4)
    (arm_up sherlock)
)
(:goal 
    (has_correct_hypo sherlock)
))
