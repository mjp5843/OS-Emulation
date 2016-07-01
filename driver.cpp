// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;

#include "scheduler.h"
#include "device.h"

int main()
{
	Scheduler *scheds[] = 	    // make some process schedulers
	{ new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new SRT() };
	Process *tasks[] = 		    // 4 processes
	{ new Computation(0), new Download(1), new Download(2), new Interact(3) };
	int arrival[] = { 0, 40, 80, 120 };   // arrive at these times

	for (int i = 0; i < 5; i++)
	{
		scheds[i]->runScheduler(tasks, arrival, 4);
		displayHistory(tasks, 4, 0, 3000);
	}
}

