In this assignment, you will implement some of the basic elements of an event-driven simulation of a system with two dependent queues. The details of the system are described below.

# Background:
You need to model a major metropolitan hospital's Emergency Department. The department has an occupancy of B total patients including R single-occupancy patient rooms and B-R patients in the waiting area. 
For example, the department can have 100 emergency rooms and 200 patients in the waiting area for a total capacity of 300 patients. The department runs 24 hours a day, 7 days a week. If the emergency 
department is at capacity (e.g., 300 patients) when a new patient arrives, the arrival is turned away and transferred to a different hospital. Patients who arrive to the emergency department are first
evaluated by triage nurses and get prioritized for receiving care based on the severity of their condition. For example, patients with critical or life-threatening conditions are given priority over adult 
patients with high pain levels and children with medium to high pain levels (but not life-threatening), and the latter group is prioritized over all other patients.

# System to Simulate:
The system consists of two queues, one after the other. Patients arriving to the emergency department wait in the evaluation "E" queue. After evaluation, they wait in a priority "P" queue until a room is 
freed up and cleaned. Note that the treatment time for a patient only includes the time they get treated in a room, but the next patient needs to wait during the time the room is being cleaned up. Patients 
in the emergency department are either inside a room or waiting in the waiting area (either in the P or E queues). The first "E" queue has m1 servers (i.e., triage nurses). The second “P” queue has R servers 
(i.e., rooms).  The cleanup operation has m2 servers (i.e., janitors). The total number of patients in the whole system should not exceed system capacity B. This maximum capacity includes patients in both 
queues and patients being treated. Patients arrive and enter the "E" queue if the system is not full. The patient at the head of each queue receives service when any of the servers (nurses or rooms) becomes 
available. Each patient has to pass both queues before departing the system.

Simulation will run for a 30-hour period. Statistics for the first 6 hours should be discarded (initial transient). You should only report statistics for the last 24 hours of simulation. 

# Queues:
You will implement one M/M/m1 queue ("E") with FCFS (First-come, First-served) service discipline, and one priority queue. We can assume that patients come from an infinite population.  Arriving patients 
leave immediately if the department is at full capacity and get transferred to a different hospital. 

# Random Variate Generation:
Visitor interarrival time and both queues’ service time distributions are exponential. Arrivals are generated based on the mean interarrival time minutes. Service time of the "E" queue is based on a service 
time distribution with mean service time 1/(mu_e) minutes. Service time of the "P" queue is based on a service time distribution with mean service time minutes depending on priority value. Additional time for 
room cleanup is based on constant time 1/(mu_c) minutes if a janitor is available. You will need to write a separate function for each distribution to generate values for arrival times and service times per 
server based on the exponential distribution. 

# Start and End of Simulation:
Simulation starts at 12 AM, and ends at 6 AM the following day. You should discard statistics from the first 6 hours of simulation and only report results from the last 24 hours. 

# Report Generation:
- Total number of departures, i.e., the total number of patients leaving the emergency department
- Average number of patients in system  
- Average response time for all patients (in minutes). Each patient's response time is the time difference between arrival time and the time they depart a room. Only consider patients who departed the emergency department for this average metric. 
- Average waiting time in the E queue (in minutes). Each patient's waiting time is the difference between arrival time and the time they start their evaluation by a nurse. You should consider all patients who start evaluation for this metric, even if they haven't finished the evaluation or started treatment. 
- Average waiting time in the P queue for all patients (in minutes). Each patient's waiting time is the difference between arrival time at the P queue (i.e., time they leave evaluation by a nurse) and the time they go to a room for treatment. You should consider all patients who start treatment for this metric, even if they haven't finished treatment or left the - emergency department.
- Average cleanup time for each room (in minutes), including waiting time for a janitor to become available.
- Number of patients who leave the system because it's at full capacity when they arrive.  
