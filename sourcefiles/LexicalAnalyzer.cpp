/*********************************
 * LexicalAnalyzer.cpp
 * UFID: 49497535
 *********************************/
 
#include <iostream>
#include "LexicalAnalyzer.h"
#include "Token.h"
#include <cstdlib>
#include <string>

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(std::string codeString) 
{
	this->codeString = codeString;
	this->size = codeString.size();
	this->readPtr = 0;
}

LexicalAnalyzer::~LexicalAnalyzer() 
{

}

Token LexicalAnalyzer::getNextToken()
{
	Token token;

	while(true)
	{	
		if(readPtr == size)
			throw "No more tokens";
		
		char ch = codeString.at(readPtr++);
		if(isspace(ch) or ch == '\t' or ch == '\n')       //in case the incoming character is a space then ignore it 
		{
			continue;
		}
		else if(isalpha(ch))                //in case the incoming character is a letter then handle it as identifier
		{
			token.value +=ch;
			while(true)
			{
				if(readPtr == size)
				{					
					break;
				}
				else
				{
					ch = codeString.at(readPtr++);
					if(isalpha(ch) or isdigit(ch) or ch == '_')
					{
						token.value +=ch;
					}
					else
					{
						readPtr--;
						break;
					}
				}
			}
			
			if(!(isKeyword(token.value)))        //test if it is a keyword, else make it identifier
			{				
				token.type = "IDENTIFIER";
			}
			else
			{
				token.type = "KEYWORD";
			}
		}
		else if(isdigit(ch))                         //in case the incoming character is a digit then handle it as integer
		{
			token.value +=ch;
			while(true)
			{
				if(readPtr == size)
				{					
					break;
				}
				else
				{
					ch = codeString.at(readPtr++);
					if(isdigit(ch))
					{
						token.value +=ch;
					}
					else
					{
						readPtr--;
						break;
					}
				}
			}
			token.type = "INTEGER";
		}
		else if (isoperator(ch))                      //in case the incoming character is operator then handle it as operator		
		{
			if(ch == '/' && codeString.at(readPtr++) == '/')
			{
				while(true)
				{
					ch = codeString.at(readPtr++);
					if(ch == '\n')
					{
						readPtr--;
						break;
					}
					else if(isalpha(ch) or isdigit(ch) or isoperator(ch) or isspace(ch) or ch=='\t'
							or ch=='\'' or ch == '(' or ch==')' or ch==';' or ch==',' or ch=='\\')
					{
						continue;
					}
				}
				continue;
			}
			else
			{
				if(ch == '/')
					readPtr--;
				token.value +=ch;
				while(true)
				{
					if(readPtr == size)
					{						
						break;
					}
					else
					{
						ch = codeString.at(readPtr++);
						if(isoperator(ch))
						{
							token.value +=ch;
						}
						else
						{
							readPtr--;
							break;
						}
					}
				}
				token.type = "OPERATOR";
			}
		}
		else if(ch == '\'')                  //in case the incoming character is part of escape sequence then handle it accordingly
		{
			token.value += ch;
			while(true)
			{
				ch = codeString.at(readPtr++);
				if(ch == '\\')
				{
					char ch1 = codeString.at(readPtr++);
					if(ch1 =='t' or ch1 == 'n' or ch1=='\\' or ch1=='\'')
					{
						token.value += ch;
						token.value += ch1;
					}
					else
					{
						throw "Problem with creating <STRING> token";
					}
				}
				else if(ch == '\'')
				{
					token.value +=ch;
					token.type ="STRING";
					return token;
				}
				else if(isalpha(ch) or isdigit(ch) or isoperator(ch) or ch==')' 
				        or ch=='(' or ch==';' or ch==',' or isspace(ch))
				{
					token.value +=ch;
				}
			}
		}
		else if(ch == '(' or ch == ')' or ch == ';' or ch == ',')         //in case the incoming character is braces or semi colon
		{
			token.type = ch;
			token.value = ch;
		}
		return token;
	}
}

Token LexicalAnalyzer::peekNextToken()
{
	if(readPtr == size)
		throw "No more tokens";
	
	Token t = getNextToken();
	int count = t.value.size();
	while(count-- !=0)
	{
		readPtr--;
	}
	return t;
}

bool LexicalAnalyzer::isoperator(char ch)
{
	char operators[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
	int i = 0;
	while(i < 26)
	{
		if(ch==operators[i])
		{
			return true;
		}
		i++;
	}
	return false;
}

bool LexicalAnalyzer::isKeyword(std::string tokenValue)
{
	std::string keywords[] = {"let","where","aug","or","not","gr","ge","ls","le","eq","ne","true",
							  "false","nil","dummy","in","fn","within","and","rec","list"};
	int i = 0;
	while(i < 21)
	{
		if(tokenValue == keywords[i])
		{
			return true;
		}
		i++;
	}
	return false;
}

void LexicalAnalyzer::operator=(const LexicalAnalyzer& laObj)
{
	this->codeString = laObj.codeString;
	this->size = laObj.size;
	this->readPtr = laObj.readPtr;
}

LexicalAnalyzer::LexicalAnalyzer()
{

}
