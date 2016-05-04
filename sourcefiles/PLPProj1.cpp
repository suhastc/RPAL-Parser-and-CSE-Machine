/**********************************
 * PLPProj1.cpp
 * UFID: 49497535
 **********************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "FileReader.h"
#include "LexicalAnalyzer.h"
#include "RecursiveParser.h"
#include "TreeTransformer.h"
#include "TreeNode.h"
#include "CSEMachine.h"

using namespace std;

void preOrder(TreeNode* t, std::string dots);
void formattedPrint(Token t,std::string dots);
void computeResult(int argc,char** argv);

int main(int argc,char** argv) 
{
	if(argc == 2)
	{
		computeResult(argc,argv);
		return 0;
	}
	
	bool astFound = false;
	int i=0;
	while(i < argc)
	{	
		if(strcmp(argv[i],"-ast") == 0)
		{
			astFound = true;
			break;
		}
		i++;
	}
	
	if(astFound == true)
	{
		FileReader fr(argv[argc-1]);
		string fileContent = fr.getFileContent();
		if(fileContent.size() == 0)
			return 0;
		LexicalAnalyzer la(fileContent);
		RecursiveParser rp(la);
		rp.parse();

		TreeNode* root = rp.getTree();
		preOrder(root, "");
		
		TreeTransformer transformer;
		TreeNode* transformedRoot = transformer.transformTree(root);
		
		CSEMachine machine(transformedRoot);
		machine.evaluateTree();
	}
	return 0;
}

void computeResult(int argc,char** argv)
{
	FileReader fr(argv[argc-1]);
	string fileContent = fr.getFileContent();
	if(fileContent.size() == 0)
		return;
	LexicalAnalyzer la(fileContent);
	RecursiveParser rp(la);
	rp.parse();

	TreeNode* root = rp.getTree();
	TreeTransformer transformer;
	TreeNode* transformedRoot = transformer.transformTree(root);

	CSEMachine machine(transformedRoot);
	machine.evaluateTree();
}

void preOrder(TreeNode* t, std::string dots)
{
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void formattedPrint(Token t,std::string dots)
{
	if(t.type == "IDENTIFIER")
	{
		cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}
	else if(t.type == "INTEGER")
	{
		cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}
	else if(t.type == "STRING")
	{
		cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}
	else if(t.value == "true" or t.value == "false" or t.value == "nil" or t.value== "dummy")
	{
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}
	else
	{
		cout <<dots<<t.value<<endl;
	}
}
