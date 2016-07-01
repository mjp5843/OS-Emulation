#include "scheduler.h"
#include "device.h"


void Scheduler::runScheduler(Process* tasks[], int arrival[], int size)
{
	int pid;			// process wanting action
	int newpid;			// newly arriving process
	char discard;
	Device *newDevice;


	for (int i = 0; i < size; i++)
	{
		future.insert(i, arrival[i], 'X');	// all want to run
		tasks[i]->restart();			// and start at beginning
		tasks[i]->addLog(arrival[i], '-');	// might have to wait
	}

	clock = 0;		// initialize the clock

	cpu.restart();
	disk.restart();
	net.restart();
	console.restart();


	while (!future.empty() || !noneReady())
	{
		if (noneReady())		// nothing avaliable now
		{				//    but something in the future
			clock = future.leadTime();		// go do that future
			future.popFront(pid, discard);	// get that task
			addProcess(pid);			// which is ready to run
		}
		else 				// else run with what we have
		{
			chooseProcess(pid);		// run this one
			tasks[pid]->run(clock, allowance(), newDevice);

			//  some tasks may have arrived in the meantime, so get those
			while (!future.empty() && clock >= future.leadTime())
			{
				future.popFront(newpid, discard);
				addProcess(newpid);
			}

			//  as far as the task that was just executing, it can
			//  work some more, after waiting its turn
			//  (which is why this appears after that last while loop)
			if (newDevice == &cpu)
				addProcess(pid);
			else if (newDevice != NULL)
				newDevice->request(pid, clock, tasks, future);
			else if (newDevice == NULL)
				tasks[pid]->addLog(clock, 'Q');
		}
	}
}


bool SRT::noneReady()
{
	if (minheapSize == 0)
		return true;
	else
		return false;
}

void SRT::addProcess(int procID)
{
	int pos;
	minheapSize++;
	pos = minheapSize;
	while ((pos > 1) && minHeap[pos / 2]->getRemainingTime() > procs[procID]->getRemainingTime())
	{
		minHeap[pos] = minHeap[pos / 2];	// move larger parent value down
		pos = pos / 2;		// and continue upwards
	}
	minHeap[pos] = procs[procID];		// insert new data	
}

void SRT::chooseProcess(int &procID)
{
	int pos;
	int child;
	bool done = false;		// not done fixing the heap
	Process *end;

	procID = minHeap[1]->getId();
	end = minHeap[minheapSize];
	minheapSize--;
	pos = 1;		// we will make rightmost leaf empty
	child = 2;			// the left child of the root
	while (child <= minheapSize && !done) // stop at heap bottom or when nothing moved
	{
		if (child < minheapSize	&& minHeap[child]->getRemainingTime() > minHeap[child + 1]->getRemainingTime())
			child = child + 1;		//      choose that one
		if (end->getRemainingTime() < minHeap[child]->getRemainingTime())	// if parent has smaller value
			done = true;			//    we are done
		else
		{
			swap(minHeap[child], minHeap[pos]);	// else swap and continue
			pos = child;			//    down the tree
			child = 2 * pos;			//       with new left child
		}
	}
	minHeap[pos] = end;
}

