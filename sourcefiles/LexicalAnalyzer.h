/********************************
 * LexicalAnalyzer.h
 * UFID: 49497535
 ********************************/

#include "Token.h"
#include <string>

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

using namespace std;

class LexicalAnalyzer 
{
	private:
		string codeString;
		int size;
		int readPtr;
		bool isoperator(char ch);
		bool isKeyword(string tokenValue);

	public:
		LexicalAnalyzer(string codeString);
		LexicalAnalyzer();
		virtual ~LexicalAnalyzer();
		
		void operator=(const LexicalAnalyzer& laObj);
		Token getNextToken();
		Token peekNextToken();
};

#endif 
