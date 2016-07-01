#include <iostream>
using namespace std;

// List of Process ProcList Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation

#include "proclist.h"

// First, some helper functions to display the data

ostream &operator<<(ostream &stream, ProcListElement &ele)
{
	stream << "(" << ele.procID << "," << ele.state << ","
		<< ele.time << ")";
	return stream;
}

ostream &operator<<(ostream &stream, ProcList &hist)
{
	for (ProcListElement *curr = hist.head; curr != NULL; curr = curr->next)
		stream << *curr;
	stream << endl;
	return stream;
}

// And some functionality for the list itself
ProcIterator ProcList::begin()
{
	return ProcIterator(this, head);
}

ProcIterator ProcList::end()
{
	return ProcIterator(this, NULL);
}

void ProcList::pushBack(int procId, int time, char state)
{
	ProcListElement *newEle = new ProcListElement(procId, time, state);
	if (head == NULL)
		head = tail = newEle;
	else
	{
		newEle->prev = tail;
		tail = tail->next = newEle;
	}
}

void ProcList::popFront(int &procId, char &state)
{
	ProcListElement *removal = head;
	head = head->next;
	if (head == NULL)
		tail = NULL;
	else
		head->prev = NULL;

	procId = removal->procID;
	state = removal->state;
	delete removal;
}

void ProcList::insert(int procId, int time, char state)
{
	ProcListElement *newEle = new ProcListElement(procId, time, state);
	if (head == NULL)
		head = tail = newEle;
	else if (time < head->time)
	{
		newEle->next = head;
		head = newEle;
	}
	else if (time >= tail->time)
	{
		newEle->prev = tail;
		tail = tail->next = newEle;
	}
	else
	{
		ProcListElement *scan;
		for (scan = head; scan->next->time < time; scan = scan->next) {}
		newEle->next = scan->next;
		newEle->prev = scan;
		scan->next->prev = newEle;
		scan->next = newEle;
	}
}

ProcIterator ProcIterator::erase()
{
	return *this;

	ProcListElement *removal = current;
	ProcIterator retval = ProcIterator(list, removal->next);

	if (removal->next == NULL)
	{
		list->tail = removal->prev;
		list->tail->next = NULL;
	}
	else
		removal->next->prev = removal->prev;
	if (removal->prev == NULL)
	{
		list->head = removal->next;
		list->tail->prev = NULL;
	}
	else
		removal->prev->next = removal->next;
	delete removal;
	return retval;
}
