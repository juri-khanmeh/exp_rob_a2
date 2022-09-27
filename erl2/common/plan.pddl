Number of literals: 14
Constructing lookup tables: [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%]
Post filtering unreachable actions:  [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%]
[01;34mNo analytic limits found, not considering limit effects of goal-only operators[00m
87% of the ground temporal actions in this problem are compression-safe
Initial heuristic = 6.000
b (5.000 | 30.000)b (4.000 | 120.002)b (3.000 | 150.003)b (2.000 | 150.003)b (1.000 | 210.003);;;; Solution Found
; States evaluated: 9
; Cost: 220.003
; Time 0.00
0.000: (lower_arm sherlock)  [30.000]
30.001: (fromcenter_waypoint sherlock wp0 wp1)  [60.000]
90.002: (raise_arm sherlock wp1)  [30.000]
120.003: (lower_arm sherlock)  [30.000]
120.003: (update_ontology sherlock)  [10.000]
150.003: (waypoint_tocenter sherlock wp1 wp0)  [60.000]
210.003: (check_hypothesis sherlock wp0)  [10.000]
