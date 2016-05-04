/***************************
 * TreeNode.cpp
 * UFID: 49497535
 ***************************/

#include "TreeNode.h"

TreeNode::TreeNode() 
{
	right = NULL;
	left = NULL;
}

TreeNode::TreeNode(Token token)
{
	this->value = token;
	right = NULL;
	left = NULL;
}

TreeNode::~TreeNode() 
{
	if(right != NULL)
		delete right;
	if(left != NULL)
		delete left;
}
