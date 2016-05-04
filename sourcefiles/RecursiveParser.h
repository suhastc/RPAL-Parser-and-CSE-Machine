/********************************
 * RecursiveParser.h
 * UFID: 49497535
 ********************************/
 
#ifndef RECURSIVEPARSER_H_
#define RECURSIVEPARSER_H_

#include "TreeNode.h"
#include "LexicalAnalyzer.h"
#include "Stack.h"
#include "Token.h"
#include <cstdlib>
#include <string>

using namespace std;

class RecursiveParser
{
	public:
		static string ID;
		static string STR;
		static string INT;
		static string KEY;
		static string OPT;
	
	private:
		LexicalAnalyzer la;
		bool moreTokens;
		Stack stack;
		Token nextToken;
		void formattedPrint(Token t, string dots);
		void preOrder(TreeNode* t, string dots);
		void read(Token token);
		void buildTree(Token token, int numOfNodes);
		
		void parseE();
		void parseEw();
		void parseT();
		void parseTa();
		void parseTc();
		void parseB();
		void parseBt();
		void parseBs();
		void parseBp();
		void parseA();
		void parseAt();
		void parseAf();
		void parseAp();
		void parseR();
		void parseRn();
		void parseD();
		void parseDa();
		void parseDr();
		void parseDb();
		void parseVb();
		void parseV1();
		
		void parseBpHelper(Token nextToken, string tokenValue);
		void parseRHelper(Token t, string value);
	
	public:
		RecursiveParser(LexicalAnalyzer la);
		~RecursiveParser();
		
		void printTree();
		void parse();
		TreeNode* getTree();
};

#endif 
