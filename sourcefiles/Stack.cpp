/*********************************
 * Stack.cpp
 * UFID: 49497535
 *********************************/

#include "Stack.h"
#include <iostream>

Stack::Stack() 
{
	top = NULL;
	size = 0;
}

Stack::~Stack() 
{
	while(top != NULL)
	{
		TreeNode* curr = top;
		top = top->next;
		delete curr;
	}
}

TreeNode* Stack::pop()
{
	if(top == NULL)
		return NULL;
	TreeNode* curr = top;
	top = top->next;
	size--;
	return curr;
}

void Stack::push(TreeNode* treeNode)
{
	treeNode->next = top;
	size++;
	top = treeNode;
}

void Stack::addRightChild(TreeNode* treeNode)
{
	top->right = treeNode;
}

int Stack::getSize()
{
	return this->size;
}
