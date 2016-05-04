/******************************
 * Token.cpp
 * UFID: 49497535
 ******************************/

#include "Token.h"
#include <vector>

using namespace std;

Token::Token() 
{
	isTuple = false;
	construct();
}

Token::Token(std::string value, std::string type)
{
	this->value = value;
	this->type = type;
	isTuple = false;
	construct();
}

Token::Token(std::string type, int envNum)
{
	this->type = type;
	this->envNum = envNum;
	isTuple = false;
	construct();
}

void Token::construct()
{
	this->tuple = vector<Token>();
}

Token::Token(std::string type,std::string lambdaParam, int lambdaNum)
{
	this->type = type;
	this->lambdaParam = lambdaParam;
	this->lambdaNum = lambdaNum;
	isTuple = false;
	construct();
}

Token::Token(std::string type, int betaIfDeltaNum, int betaElseDeltaNum)
{
	this->type = type;
	this->betaIfDeltaNum = betaIfDeltaNum;
	this->betaElseDeltaNum = betaElseDeltaNum;
	isTuple = false;
	construct();
}

Token::~Token() 
{

}
