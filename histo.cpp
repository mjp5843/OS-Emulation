// History Displayer
// Produces an Ascii-graphic representation of the history of a process
// within a simulation of job scheduling.  The history is recorded as a
// series of time stamps, represented here as integer time values and
// characters representing state.
// Paramaters:
//     history	input procList array	linked lists of events and times
//     size	input int		number of elements in array
//     start	input int		beginning of time frame of interest
//     stop	input int		end of time frame of interest
// Pre-Conditions:
//     The elements in 'history' are in non-decreasing order
//     'history' ends with an entry with 'Q' as its state
//     There is at least one entry before the 'Q'
//     'start' and 'stop' are both positive integers, with start < stop
//     NOTE:  either 'start' or 'stop' may exceed the largest time in 'history'
// Results:
//     A display of between 20 and 50 printable characters representing
//     the job history within the time range of interest, using the
//     characters stored within 'history'.  Events outside the actual 
//     range of the process history will be displayed with blanks.

#include <iostream>
#include <iomanip>
using namespace std;

#include "process.h"

void displayHistory(Process* history[], int size, int start, int stop)
{
	char display[51];			// to hold the output
	int outpos;				// next output position to fill

	char  currentState;			// current process state in history
	int time;				// current time examined
	int range = stop - start;		// total time period
	int increment = 1 + range / 50;	// round fractions upwards


	cout << "Time:  " << setw(4) << start <<
		setw(range / increment) << stop << endl;

	for (int j = 0; j < size; j++)
	{
		for (int i = 0; i < 50; i++)		// clear display
			display[i] = ' ';

		ProcList &pl = history[j]->getLog();	// find out about this task
		ProcIterator iter = pl.begin();	// starting at its first point
										
										//  Identify where to begin recording output data, and
										//  what time to begin analyzing (tracking may start early)
		iter = pl.begin();
		if (iter.time() > start)		// has not yet started yet
			outpos = (iter.time() - start) / increment;	// skip spaces
		else
			outpos = 0;				// start at beginning of display
		time = start + outpos*increment;	// identify smulation time

											//  Identify where first applicable at appears
											//  (tracking may start in the middle)
		currentState = iter.state();	// initialize if loop never repeats
		while (time <= stop && time >= iter.time())
		{
			currentState = iter.state();	// 
			iter.advance();
		}
		//  currentState represents the process state at 'time'
		//  and changes from that state at iter.time()

		// collect relevant data into the output array
		while (time <= stop && currentState != 'Q')
		{

			// currentState extends to iter.time(), so record it until
			// that time is reached.

			while (time <= stop && time < iter.time())
			{
				display[outpos] = currentState;
				outpos++;
				time += increment;
			}

			currentState = iter.state();	// change to this state
			iter.advance();			// and find when it ends


									// It is possible that the last increment jumped over a state
									// so well will record any missing X's in last output position
									// It is also very possible that many states got missed,
									// so we will attempt to catch all of them
			while (currentState != 'Q' && time >= iter.time())
			{
				if (currentState == 'X')	    // missed this execution
					display[outpos - 1] = 'X';	// record very short CPU burst
				currentState = iter.state();	// what did we change to?
				iter.advance();		        	// find where that ends
			}

		}

		display[outpos] = '\0';
		cout << "History: " << display << endl;
	}

	double avTurnTime = 0;  // Average Turnaround Time, for non-interactive processes

	int turnTime = 0, turnCounter = 0;
	int inter_time = 0, inter_start, max = 0;
	int interactCounter = 0;
	int totalResponse;

	for (int n = 0; n < size; n++)
	{
		ProcList &pl = history[n]->getLog();			//get info on task
		if (history[n]->isInteractive() == true)		//find out if interactive
		{
			ProcIterator iter = pl.begin();				//start at beginning of task
			while (iter.state() != 'I')					//find first time process is interactive
			{
				iter.advance();
			}
			do {
				iter.advance();							//move past interactive process
				inter_start = iter.time();
				while (iter.state() != 'I' && iter.state() != 'Q')		//find next time process is interactive again
				{
					iter.advance();
				}
				if (iter.state() == 'I')
				{
					totalResponse = iter.time() - inter_start;			//calculates time of response
					inter_time += totalResponse;
					interactCounter++;
					if (totalResponse > max)						//if response time is larger than all other response times, max will be replaced by it
						max = totalResponse;
				}
			} while (iter.state() == 'I');
		}
		//find turnaround time for noninteractive processes
		else
		{
			turnTime += (pl.tailTime() - pl.leadTime());
			turnCounter++;
		}
	}
	avTurnTime = (turnTime / turnCounter);
	cout << "Average Turnaround Time: " << avTurnTime << endl;
	cout << "Average Response Time: " << inter_time / interactCounter << endl;
	cout << "Maximum Response Time: " << max << endl << endl;
}

/*
NOTE:
Output for all values are much too high,
but unable to determine why even after
visiting office hours.
*/