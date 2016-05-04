/******************************
 * RecursiveParser.cpp
 * UFID: 49497535
 ******************************/
 
#include <iostream>
#include "RecursiveParser.h"
#include "Token.h"

using namespace std;

string RecursiveParser::ID = "IDENTIFIER";
string RecursiveParser::STR = "STRING";
string RecursiveParser::INT = "INTEGER";
string RecursiveParser::KEY = "KEYWORD";
string RecursiveParser::OPT = "OPERATOR";

RecursiveParser::RecursiveParser(LexicalAnalyzer la)
{
	this->la = la;
	this->moreTokens = true;
}

RecursiveParser::~RecursiveParser()
{

}

void RecursiveParser::read(Token token)
{
	if(moreTokens == false)
		throw "Error: No more token to parse";
	if(token.value != nextToken.value)
	{
		exit(1);
	}
	if(token.type == ID or token.type == STR or token.type == INT)
	{
		buildTree(token, 0);
	}
	try
	{
		nextToken = la.getNextToken();
	}
	catch(const char* message)
	{
		moreTokens = false;
		Token endToken("$$","$$");
		nextToken = endToken;
	}
}

void RecursiveParser::buildTree(Token token, int numOfNodes)
{
	TreeNode* newNode = new TreeNode;
	newNode->value = token;
	if(numOfNodes != 0)
	{
		int i=0;
		while(i < numOfNodes-1)
		{		
			TreeNode* curr = stack.pop();
			stack.addRightChild(curr);
			i++;
		}
		TreeNode* top = stack.pop();
		if(top != NULL)
			newNode->left = top;
	}
	stack.push(newNode);
}

void RecursiveParser::parseE()
{
	if(nextToken.value == "let")
	{
		Token letToken("let",KEY);
		read(letToken);
		parseD();
		Token inToken("in",KEY);
		read(inToken);
		parseE();
		Token nodeToken("let","let");
		buildTree(nodeToken,2);
	}
	else if(nextToken.value == "fn")
	{
		Token fnToken("fn","fn");
		read(fnToken);
		int n=0;
		do
		{
			parseVb();
			n++;
		}while(nextToken.type == ID or nextToken.type == "(");
		Token dotToken(".",OPT);
		read(dotToken);
		parseE();
		Token nodeToken("lambda","lambda");
		buildTree(nodeToken,n+1);
	}
	else
	{
		parseEw();
	}	
}

void RecursiveParser::parseEw()
{
	parseT();
	if(nextToken.value == "where")
	{
		Token t("where",KEY);
		read(t);
		parseDr();
		Token nodeToken("where","where");
		buildTree(nodeToken,2);
	}	
}

void RecursiveParser::parseT()
{
	parseTa();
	if(nextToken.value == ",")
	{
		int n = 0;
		do
		{
			read(nextToken);
			parseTa();
			n++;
		}while(nextToken.value == ",");
		Token tauToken("tau","tau");
		tauToken.tauCount = n+1;
		buildTree(tauToken,n+1);
	}	
}

void RecursiveParser::parseTa()
{
	parseTc();
	while(nextToken.value == "aug")
	{
		Token temp = nextToken;
		read(nextToken);
		parseTc();
		buildTree(temp,2);
	}	
}

void RecursiveParser::parseTc()
{
	parseB();
	if(nextToken.value == "->")
	{
		read(nextToken);
		parseTc();
		Token elseToken("|",OPT);
		read(elseToken);
		parseTc();
		Token nodeToken("->","->");
		buildTree(nodeToken,3);
	}	
}

void RecursiveParser::parseB()
{
	parseBt();
	while(nextToken.value == "or")
	{
		nextToken.type = RecursiveParser::OPT;
		Token temp = nextToken;
		read(nextToken);
		parseBt();
		buildTree(temp,2);
	}	
}

void RecursiveParser::parseBt()
{
	parseBs();
	while(nextToken.value == "&")
	{
		nextToken.type = RecursiveParser::OPT;
		Token temp = nextToken;
		read(nextToken);
		parseBs();
		buildTree(temp,2);
	}	
}

void RecursiveParser::parseBs()
{
	if(nextToken.value != "not")
	{		
		parseBp();
	}
	else
	{
		nextToken.type = "not";
		Token temp = nextToken;
		read(nextToken);
		parseBp();
		buildTree(temp,1);
	}	
}

void RecursiveParser::parseBp()
{
	parseA();
	if(nextToken.value=="gr" or nextToken.value == ">")
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken, "gr");
	}
	else if(nextToken.value == "ls" or nextToken.value == "<")
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken,"ls");
	}
	else if(nextToken.value =="ge" or nextToken.value == ">=" )
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken, "ge");
	}
	else if(nextToken.value =="le" or nextToken.value == "<=" )
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken, "le");
	}
	else if(nextToken.value =="eq" )
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken, "eq");
	}
	else if(nextToken.value =="ne")
	{
		nextToken.type = RecursiveParser::OPT;
		parseBpHelper(nextToken, "ne");
	}	
}

void RecursiveParser::parseBpHelper(Token nextToken, string tokenValue)
{
	read(nextToken);
	parseA();
	Token t( tokenValue,RecursiveParser::OPT);
	buildTree(t,2);
}

void RecursiveParser::parseA()
{
	if(nextToken.value == "-")
	{
		Token negToken("-",OPT);
		read(negToken);
		parseAt();
		Token nodeToken("neg","neg");
		buildTree(nodeToken,1);
	}
	else if(nextToken.value == "+")
	{
		Token posToken("+",OPT);
		read(posToken);
		parseAt();
	}
	else
	{
		parseAt();
	}
	while(nextToken.value == "+" or nextToken.value == "-")
	{
		Token temp = nextToken;
		read(nextToken);
		parseAt();
		buildTree(temp,2);
	}	
}

void RecursiveParser::parseAt()
{
	parseAf();
	while(nextToken.value == "*" or nextToken.value=="/")
	{
		Token temp = nextToken;
		read(nextToken);
		parseAf();
		buildTree(temp,2);
	}
}

void RecursiveParser::parseAf()
{
	parseAp();
	while(nextToken.value == "**")
	{
		Token temp = nextToken;
		read(nextToken);
		parseAf();
		buildTree(temp,2);
	}	
}

void RecursiveParser::parseAp()
{
	parseR();
	while(nextToken.value == "@")
	{
		Token temp = nextToken;
		read(nextToken);
		if(nextToken.type != ID)
			throw "Expected Identifier found in parseAp()";
		read(nextToken); 
		parseR();
		buildTree(temp,3);
	}	
}

void RecursiveParser::parseR()
{
	parseRn();
	while(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT or
	      nextToken.value == "true" or nextToken.value == "false" or nextToken.value == "nil" or
		  nextToken.value == "(" or nextToken.value == "dummy")
	{
		parseRn();
		Token nodeToken("gamma","gamma");
		buildTree(nodeToken,2);
	}	
}

void RecursiveParser::parseRn()
{
	if(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT )
	{
			read(nextToken);
	}
	else if(nextToken.value=="true")
	{
		parseRHelper(nextToken, "true");
	}
	else if(nextToken.value == "false")
	{
		parseRHelper(nextToken, "false");
	}
	else if(nextToken.value == "nil")
	{
		parseRHelper(nextToken, "nil");
	}
	else if(nextToken.value == "dummy")
	{
		parseRHelper(nextToken,"dummy");
	}
	else if(nextToken.value == "(")
	{
		read(nextToken);
		parseE();
		Token t(")",")");
		read(t);
	}	
}

void RecursiveParser::parseRHelper(Token t, string value)
{
	read(t);
	Token nodeToken(value,value);
	buildTree(nodeToken,0);
}

void RecursiveParser::parseD()
{
	parseDa();
	if(nextToken.value == "within")
	{
		read(nextToken);
		parseD();
		Token nodeToken("within", "within");
		buildTree(nodeToken,2);
	}	
}

void RecursiveParser::parseDa()
{
	parseDr();
	if(nextToken.value== "and")
	{
		int n = 1;
		Token temp = nextToken;
		while(nextToken.value == "and")
		{
			read(nextToken);
			parseDr();
			n++;
		}
		buildTree(temp,n);
	}	
}

void RecursiveParser::parseDr()
{
	if(nextToken.value != "rec")
	{		
		parseDb();
	}
	else
	{
		Token temp = nextToken;
		read(nextToken);
		parseDb();
		buildTree(temp,1);
	}	
}

void RecursiveParser::parseDb()
{
	if(nextToken.value == "(")
	{
		read(nextToken);
		parseD();
		Token t(")",OPT);
		read(t);
	}
	else if(nextToken.type == ID && (la.peekNextToken().value == "," || la.peekNextToken().value == "="))
	{
		parseV1();
		Token t("=",OPT);
		read(t);
		parseE();
	
		Token nodeToken("=","=");
		buildTree(nodeToken,2);
	
	}
	else
	{
		read(nextToken);
		int n = 1;
		parseVb();
		while(nextToken.type == ID or nextToken.value== "(")
		{
			n++;
			parseVb();
		}
		Token t("=",OPT);
		read(t);
		parseE();
		Token nodeToken("function_form", "function_form");
		buildTree(nodeToken, n+2);
	}	
}

void RecursiveParser::parseVb()
{
	if(nextToken.type == ID)
	{
		read(nextToken);
	}
	else if(nextToken.value == "(")
	{
		read(nextToken);
		if(nextToken.value != ")")
		{						
			parseV1();
			Token t(")",")");
			read(t);
		}
		else
		{
			read(nextToken);
			Token nodeToken("()","()");
			buildTree(nodeToken,0);
		}
	}	
}

void RecursiveParser::parseV1()
{
	read(nextToken);
	int n =1;
	if(nextToken.value == ",")
	{
		while(nextToken.value == ",")
		{
			n++;
			read(nextToken); 
			read(nextToken); 
		}
		Token nodeToken(",",",");
		buildTree(nodeToken,n);
	}
}

void RecursiveParser::printTree()
{
	TreeNode* t = stack.pop();
	stack.push(t);
	preOrder(t,std::string(""));
}

void RecursiveParser::preOrder(TreeNode* t, std::string dots)
{
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void RecursiveParser::formattedPrint(Token t,std::string dots)
{
	
}

void RecursiveParser::parse()
{
	nextToken = la.getNextToken();
	parseE();
}

TreeNode* RecursiveParser::getTree()
{
	return stack.pop();
}
