/**********************************
 * CSEMachine.cpp
 * UFID: 49497535
 **********************************/

#include "CSEMachine.h"
#include "TreeNode.h"
#include "RecursiveParser.h"
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <utility>

typedef pair<int,string> key_pair;

CSEMachine::CSEMachine() 
{

}

CSEMachine::~CSEMachine() 
{

}

CSEMachine::CSEMachine(TreeNode* input)
{
	this->inputTree = input;
	this->deltaCounter = 0;
	this->currDeltaNum = 0;
	this->envCounter = 0;
	this->envStack.push(0);
	this->currEnv = 0;
	this->envMap = map<int,int>();
	this->printCalled = false;
}

void CSEMachine::evaluateTree()
{
	createControlStructures(this->inputTree);
	Token envToken("env",envCounter);
	stack<Token> controlStack;
	stack<Token> executionStack;
	controlStack.push(envToken);
	
	envMap[0] = -1;   //Set parent of env[0] to -1

	vector<Token> delta0 = deltaMap[0];
	int i=0;
	while(i<delta0.size())
	{
		controlStack.push(delta0[i]);
		i++;
	}
	executionStack.push(envToken);
	
	int whileCount = 0;
	
	while(controlStack.size() != 1)
	{
		Token currToken = controlStack.top();
		controlStack.pop();
		processCurrentToken(currToken,controlStack,executionStack);
	}
	
	if(printCalled == false)
	{
		cout<<endl;
	}
}

void CSEMachine::processCurrentToken(Token &currToken,stack<Token> &controlStack, stack<Token> &executionStack)
{
	if(currToken.type == RecursiveParser::OPT)
	{
		Token firstToken = executionStack.top();
		executionStack.pop();
		Token secondToken = executionStack.top();
		executionStack.pop();
		Token resultToken = applyOperator(firstToken, secondToken, currToken);		
		executionStack.push(resultToken);
	}
	else if(currToken.type == "neg")
	{
		Token firstToken = executionStack.top();
		executionStack.pop();
		int paramVal = atoi(firstToken.value.c_str());
		paramVal = -paramVal;
		Token newToken(intToString(paramVal), RecursiveParser::INT);
		executionStack.push(newToken);
	}
	else if(currToken.type =="not")
	{
		Token firstToken = executionStack.top();
		executionStack.pop();
		if(firstToken.value != "true")
		{			
			executionStack.push(Token("true","true"));
		}
		else
		{
			executionStack.push(Token("false","false"));
		}
	}
	else if(currToken.type == RecursiveParser::ID && isParamter(currToken))
	{
		string varName = currToken.value;
		int temp = currEnv;
	
		pair<int,string> keyPair(temp,varName);
		map<key_pair,Token>::iterator it = paramMap.find(keyPair);
		while(paramMap.end() == it && temp>=0)
		{
			temp = envMap[temp];
			keyPair.first = temp;		
			it = paramMap.find(keyPair);
		}
		if(paramMap.end() != it)
		{
			Token paramValToken = it->second;		
			executionStack.push(paramValToken);
		}
	}
	else if(currToken.type == "gamma")
	{
		Token topExeToken = executionStack.top();
		executionStack.pop();
		if(topExeToken.type == "lambdaClosure")
		{
			Token env("env",++envCounter);		
			envMap[envCounter] = topExeToken.lambdaEnv;
			envStack.push(envCounter);
			currEnv = envCounter;
			if(topExeToken.isTuple == false)
			{
				string paramName = topExeToken.lambdaParam;
				Token paramToken = executionStack.top();
				executionStack.pop();			
				pair<int,string> keyPair(envCounter,paramName);
				paramMap[keyPair] = paramToken;				
			}
			else
			{			
				string tuple = topExeToken.lambdaParam;
				vector<string> params = split(tuple,',');
				Token valueTuple = executionStack.top();			
				executionStack.pop();
				vector<Token> tupleVector = valueTuple.tuple;
				unsigned int i=0;				
				
				while(i<params.size())
				{
					if(params[i] != "")
					{
						pair<int,string> keyPair(envCounter,params[i].c_str());
						paramMap[keyPair] = tupleVector[i];
					}				
					i++;	
				}				
			}
			controlStack.push(env);
			executionStack.push(env);
			int lambdaNum = topExeToken.lambdaNum;
			vector<Token> delta = deltaMap[lambdaNum];
			int i=0;
			
			while(i<delta.size())
			{
				controlStack.push(delta[i]);
				i++;
			}
		}
		else if(topExeToken.type == "YSTAR")
		{
			Token nextToken = executionStack.top();		
			executionStack.pop();
			nextToken.type ="eta";
			executionStack.push(nextToken);
		}
		else if(topExeToken.type == "eta")
		{
			Token lambdaToken = topExeToken;
			lambdaToken.type = "lambdaClosure";
			executionStack.push(topExeToken);
			executionStack.push(lambdaToken);
		
			Token gammaToken("gamma","gamma");
			controlStack.push(gammaToken);
			controlStack.push(gammaToken);
		}
		else if(topExeToken.value == "Stern" || topExeToken.value == "stern")
		{
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(2,tokenValue.size()-3);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}
		else if(topExeToken.value == "Stem" || topExeToken.value == "stem")
		{
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(1,1);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}
		else if(topExeToken.value == "Conc" || topExeToken.value == "conc")
		{
			Token firstToken = executionStack.top();
			executionStack.pop();
			Token secondToken = executionStack.top();
			executionStack.pop();		
			string concatValue = firstToken.value.substr(1,firstToken.value.size()-2)+secondToken.value.substr(1,secondToken.value.size()-2);
			concatValue = "'"+concatValue+"'";			
			Token newToken(concatValue,RecursiveParser::STR);
			executionStack.push(newToken);			
			controlStack.pop();
		}
		else if(topExeToken.value == "ItoS" || topExeToken.value == "itos")
		{
			Token firstToken = executionStack.top();
			executionStack.pop();
			firstToken.type = RecursiveParser::STR;
			firstToken.value = "'"+firstToken.value+"'";
			executionStack.push(firstToken);		
		}
		else if(topExeToken.value == "Print" || topExeToken.value == "print")
		{
			printCalled = true;		
			Token t = executionStack.top();
			executionStack.pop();
			if(t.isTuple != true)
			{
				if(t.type== RecursiveParser::STR)
				{
					string tempStr =unescape(t.value.substr(1,t.value.size()-2));
					cout << tempStr;
					if(tempStr[tempStr.size()-1] == '\n')
						cout<<endl;				
				}
				else if(t.type == "lambdaClosure")
				{
					cout <<"[lambda closure: "<<t.lambdaParam<<": "<<t.lambdaNum<<"]";
				}
				else
				{				
					cout<<t.value;
				}
				Token dummyToken("dummy","dummy");
				executionStack.push(dummyToken);
			}
			else
			{
				vector<Token> tupleVector = t.tuple;
				int i=0;
			
				while(i<tupleVector.size())
				{
					if(i!=0)
					{						
						cout<<", ";
					}
					else
					{
						cout<<"(";
					}
					if(tupleVector[i].type == RecursiveParser::STR)
					{
						cout<< unescape(tupleVector[i].value.substr(1,tupleVector[i].value.size()-2));
					}
					else if(tupleVector[i].isTuple == true )
					{
						cout<<"Inside else if"<<endl;
						vector<Token> innerTuple = tupleVector[i].tuple;
						cout << "Size" << innerTuple.size()<<endl;
						if(innerTuple.size() == 1)
						{
							if(innerTuple[0].type == RecursiveParser::STR)
								cout<< unescape(innerTuple[0].value.substr(1,innerTuple[0].value.size()-2));
						}
					}
					else
					{
						cout << tupleVector[i].value;
					}
					if(i==tupleVector.size() -1)
					{
						cout<<")";
					}
					i++;
				}
			}			
		}
		else if(topExeToken.value == "Isinteger")
		{
			Token t = executionStack.top();
			executionStack.pop();
			if (t.type == RecursiveParser::INT)
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Istruthvalue")
		{
			Token t = executionStack.top();
			executionStack.pop();
			if (t.value=="true" || t.value=="false")
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Isstring")
		{
			Token t = executionStack.top();
			executionStack.pop();
			if (t.type==RecursiveParser::STR)
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Istuple")
		{		
			Token t = executionStack.top();
			executionStack.pop();
			if (t.isTuple==true)
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Isdummy")
		{
			Token t = executionStack.top();
			executionStack.pop();
			if (t.value=="dummy")
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Isfunction")
		{
			Token t = executionStack.top();
			executionStack.pop();
			if (t.type=="lambdaClosure")
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.value == "Order")
		{		
			Token t = executionStack.top();
			executionStack.pop();			
			executionStack.push(Token(intToString(t.tuple.size()),RecursiveParser::INT));
		}
		else if(topExeToken.value == "Null")
		{		
			Token t = executionStack.top();
			executionStack.pop();
			if (t.value == "nil")
				executionStack.push(Token("true","true"));
			else
				executionStack.push(Token("false","false"));
		}
		else if(topExeToken.isTuple == true)
		{
			Token t = executionStack.top();
			executionStack.pop();
			if(t.type == RecursiveParser::INT)
			{
				int indx = atoi(t.value.c_str());
				indx -=1;
				executionStack.push(topExeToken.tuple[indx]);
			}
		}
	}
	else if(currToken.type == "env")
	{
		Token topToken = executionStack.top();
		executionStack.pop();
		executionStack.pop();
		executionStack.push(topToken);
		envStack.pop();
		currEnv = envStack.top();
	}
	else if(currToken.type == "beta")
	{
		Token topToken = executionStack.top();
		executionStack.pop();
		vector<Token> delta;
		unsigned int i=0;
		
		if(topToken.value != "true")
		{			
			delta = deltaMap[currToken.betaElseDeltaNum];
		}
		else
		{
			delta = deltaMap[currToken.betaIfDeltaNum];
		}
		
		while(i<delta.size())	
		{
			controlStack.push(delta[i]);
			i++;
		}
	}
	else if(currToken.value == "tau")
	{
		int tauCount = currToken.tauCount;	
		string tuple="(";
		vector<Token> tupleVector;
		int i=0;
		
		while(i<tauCount)	
		{
			Token t = executionStack.top();
			tupleVector.push_back(t);
			executionStack.pop();
			if(i != tauCount -1)				
				tuple += t.value +", ";
			else
				tuple += t.value;
			i++;
		}
		tuple +=")";		
		Token newToken(tuple,"tuple");
		newToken.tuple = tupleVector;
		newToken.isTuple = true;
		executionStack.push(newToken);
	}
	else if(currToken.value == "nil")
	{
		currToken.isTuple = true;
		executionStack.push(currToken);
	}
	else if(currToken.value == "aug")
	{	
		Token tuple = executionStack.top();
		executionStack.pop();
		Token toAdd = executionStack.top();
		executionStack.pop();
		if(tuple.value == "nil")
		{		
			Token newToken(toAdd.value,"tuple");
			newToken.isTuple = true;
			newToken.tuple = vector<Token>();
			newToken.tuple.push_back(toAdd);
			executionStack.push(newToken);
		}
		else
		{
			tuple.tuple.push_back(toAdd);
			executionStack.push(tuple);
		}
	}
	else if(currToken.type == "lambdaClosure")
	{	
		currToken.lambdaEnv = currEnv;
		executionStack.push(currToken);
	}
	else
	{
		executionStack.push(currToken);
	}
}

Token CSEMachine::applyOperator(Token firstToken, Token secondToken, Token currToken)
{
	string tokenVal = currToken.value;
	if(firstToken.type == RecursiveParser::INT)
	{
		int firstVal = atoi(firstToken.value.c_str());
		int secondVal = atoi(secondToken.value.c_str());	
		int resultVal = 0;
		if(tokenVal == "*")
		{
			resultVal = firstVal*secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}
		else if(tokenVal == "+")
		{
			resultVal = firstVal+secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}
		else if(tokenVal == "-")
		{
			resultVal = firstVal-secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}
		else if(tokenVal == "/")
		{
			resultVal = firstVal/secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}
		else if(tokenVal == "**")
		{
			resultVal = pow(firstVal,secondVal);
			return Token(intToString(resultVal),firstToken.type);
		}
		else if(tokenVal == "gr")
		{
			if (firstVal > secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "ls")
		{		
			if (firstVal < secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "ge")
		{
			if (firstVal >= secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "le")
		{
			if (firstVal <= secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "eq")
		{
			if (firstVal == secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "ne")
		{
			if (firstVal != secondVal)
				return Token("true","true");
			else 
				return Token("false","false");
		}
	}
	else if(firstToken.type == RecursiveParser::STR)     // for handling string opertors
	{ 
		if(tokenVal == "eq")
		{
			if (firstToken.value == secondToken.value)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "ne")
		{
			if (firstToken.value != secondToken.value)
				return Token("true","true");
			else 
				return Token("false","false");
		}
	}
	else if(firstToken.type == "true" || firstToken.type == "false")     // for handling bool opertors
	{ 
		if(tokenVal == "or")
		{		
			if (firstToken.type=="true" || secondToken.type=="true")
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "&")
		{			
			if (firstToken.type=="true" && secondToken.type=="true")
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "eq")
		{
			if (firstToken.type==secondToken.type)
				return Token("true","true");
			else 
				return Token("false","false");
		}
		else if(tokenVal == "ne")
		{
			if (firstToken.type != secondToken.type)
				return Token("true","true");
			else 
				return Token("false","false");			
		}
	}
	return Token("","");
}

void CSEMachine::createControlStructures(TreeNode* root)
{	
	pendingDeltaQueue.push(root);
	while(!pendingDeltaQueue.empty())
	{
		vector<Token> currentDelta;
		TreeNode* currStartNode = pendingDeltaQueue.front();
		pendingDeltaQueue.pop();
		preOrderTraversal(currStartNode, currentDelta);
		deltaMap[currDeltaNum++] = currentDelta;
	}	
}

void CSEMachine::preOrderTraversal(TreeNode* root, vector<Token> &currentDelta)
{
	if(root->value.type == "lambda")
	{
		if(root->left->value.value != ",")
		{
			Token lambdaClosure("lambdaClosure",root->left->value.value, ++deltaCounter);
			currentDelta.push_back(lambdaClosure);
		}
		else
		{
			TreeNode* commaChild = root->left->left;
			string tuple;
			while(commaChild != NULL)
			{
				tuple += commaChild->value.value + ",";
				commaChild = commaChild->right;
			}
			Token lambdaClosure("lambdaClosure",tuple, ++deltaCounter);
			lambdaClosure.isTuple = true;
			currentDelta.push_back(lambdaClosure);
		}
		
		pendingDeltaQueue.push(root->left->right);
		if(root->right !=NULL)
			preOrderTraversal(root->right,currentDelta);
	}
	else if(root->value.value == "->")
	{
		Token betaToken("beta",deltaCounter+1,deltaCounter+2);
		currentDelta.push_back(betaToken);
		pendingDeltaQueue.push(root->left->right);
		pendingDeltaQueue.push(root->left->right->right);

		root->left->right->right = NULL;
		root->left->right = NULL;
		
		deltaCounter +=2;
		if(root->left != NULL)
			preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
			preOrderTraversal(root->right,currentDelta);
	}
	else
	{
		currentDelta.push_back(root->value);
		if(root->left != NULL)
			preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
			preOrderTraversal(root->right,currentDelta);
	}
}

string CSEMachine::intToString(int intValue)
{
	ostringstream oss;
	oss<<intValue;
	return oss.str();
}

vector<string> CSEMachine::split(string inputString, char delimiter)
{
	vector<string> result;
	string::iterator it;
	string temp;
	it = inputString.begin();
	
	while(it < inputString.end())
	{
		if(*it != delimiter)
		{						
			temp += *it;
		}
		else
		{
			result.push_back(temp);
			temp = string();
		}
		it++;
	}
	return result;
}

bool CSEMachine::notFunction(string identifier)
{
	string functions[] = {"Stem","stem","Stern","stern","Print","print","Conc","conc","Istuple",
	                      "Isinteger","Isfunction","Istruthvalue","Isdummy","Order","Null"};
	int i = 0;
	while(i < 15)
	{
		if(identifier == functions[i])
			return false;
		i += 1;
	}
	return true;
}

bool CSEMachine::isParamter(Token currToken)
{
	string varName = currToken.value;
	int temp = currEnv;
	pair<int,string> keyPair(temp,varName);
	map<key_pair,Token>::iterator it = paramMap.find(keyPair);
	
	while(paramMap.end() == it && temp>=0)
	{
		temp = envMap[temp];
		keyPair.first = temp;	
		it = paramMap.find(keyPair);
	}
	
	if(paramMap.end() == it)
	{		
		return false;
	}
	else
	{
		return true;
	}
}

string CSEMachine::unescape(const string& s)     //for handling escape sequences in input
{
	string res;
  	string::const_iterator it = s.begin();
  	while (it != s.end())
  	{
    	char c = *it++;
    	if (c == '\\' && it != s.end())
    	{
	     	switch (*it++) 
		  	{
		    	case '\\': c = '\\'; break;
		      	case 'n': c = '\n'; break;
		      	case 't': c = '\t'; break;	      	
		      	default:
			    continue;
	      	}
    	}
    	res += c;
  	}
	return res;
}
