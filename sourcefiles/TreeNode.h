/***********************************
 * TreeNode.h
 * UFID: 49497535
 ***********************************/

#ifndef TREENODE_H_
#define TREENODE_H_

#include "Token.h"

using namespace std;

class TreeNode 
{
	public:
		TreeNode();
		TreeNode(Token token);
		virtual ~TreeNode();
		Token value;
		TreeNode* left;
		TreeNode* right;
		TreeNode* next;
};

#endif

