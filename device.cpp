	#include "device.h"
	#include "process.h"

	//  Simulate a request to an I/O device
	//  Parameters:
	//      pid	(input int)		which process is making the request
	//      clock	(input int)		time at which request is made
	//      tasks	(modified array)	Process information (to record events)
	//      future	(modified ProcList)	record when operation is complete
	void Device::request(int pid, int clock, Process *tasks[], ProcList &future)
	{

		if (readyTime < clock)
			readyTime = clock;

		tasks[pid]->addLog(readyTime, action);
		readyTime += duration;
		tasks[pid]->addLog(readyTime, '-');
		future.insert(pid, readyTime, 'X');

	}

	//  Each device records a letter to be used in the summary display
	//  and the time the operation is expected to take
		Device disk('D', 200), 	// disk has 'slow' moving parts
		net('N', 100), 	// networks are faster nowadays
		console('I', 1000), 	// have to wait for user to respond
		cpu('X', 0);		// not external, but used for type compatibility
						//  When each process runs with the CPU, it will use this list to identify
						//  what it wishes to do next (using the cpu object to continue running)
