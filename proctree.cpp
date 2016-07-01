// Process Tree Implementation File
// A tree is chosen here to organize processes by priority,
// where the priority is assumed (for simplicity) to simply
// be identical to the process identifier (a small integer)
//
// The caller will simply be recording the process ID's in this tree,
// and choose the maximum value to favor processes with larger ID's.
//
#include <iostream>
#include <string>
using namespace std;

#include "proctree.h"

//  recursiveInsert
//  A recursive tree-traversal function to insert a new value
//  If the value is already present, no change is made
//  Parameters:
//  	node	(modified TreeNode ptr)	root of sub-tree to search
//  	val	(input int)		value to insert
//  NOTE: node May be a null pointer, indicating an empty tree
//  Post-condition:
//      return value points at a node with the given name,
//      even if it had to be created to do so
void ProcTree::recursiveInsert(ProcTreeNode *&node, int val)
{
	if (node == NULL)		// insert here
		node = new ProcTreeNode(val);
	else if (val < node->procID)
		recursiveInsert(node->left, val);
	else
		recursiveInsert(node->right, val);
}

//  insert
//  Inserts a new value into the binary search tree
//  Parameters:
//  	val 	(input int)		value to insert
void ProcTree::insert(int val)
{
	recursiveInsert(root, val);
}

//  recursiveRemoveMax
//  A recursive tree-traversal function to identify and remove
//  a maximum value from a binary search tree
//  Parameters:
//  	node	(modified TreeNode ptr)	root of sub-tree to search
//  	val	(input int)		value to insert
//  Pre-condition:
//      The tree is assumed to be non-empty
int ProcTree::recursiveRemoveMax(ProcTreeNode *&node)
{
	if (node->right != NULL)		// not yet maximum
		return recursiveRemoveMax(node->right);
	else
	{
		ProcTreeNode *removal = node;	// this is it
		int returnVal = node->procID;
		node = node->left;		// there may be a sub-tree
		delete removal;			// safe to deallocate now
		return returnVal;
	}
}

//  removeMax
//  identifies and removes the largest value in the tree
int ProcTree::removeMax()
{
	return recursiveRemoveMax(root);
}


ostream& operator<<(ostream& stream, ProcTreeNode &t)
{
	if (t.left != NULL)	stream << *t.left;
	stream << t.procID << " ";
	if (t.right != NULL) stream << *t.right;
	return stream;
}

ostream& operator<<(ostream& stream, ProcTree &t)
{
	return stream << *t.root;
}
