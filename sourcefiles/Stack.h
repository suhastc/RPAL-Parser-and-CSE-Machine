/**********************************
 * Stack.h
 * UFID: 49497535
 **********************************/

#ifndef STACK_H_
#define STACK_H_

#include "TreeNode.h"

using namespace std;

class Stack 
{
	private:
		TreeNode* top;
		int size;
	
	public:
		Stack();
		virtual ~Stack();
		TreeNode* pop();
		void push(TreeNode* treeNode);
		void addRightChild(TreeNode* treeNode);
		int getSize();
};

#endif
