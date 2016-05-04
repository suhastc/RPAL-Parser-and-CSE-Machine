/**************************************
 * Token.h
 * UFID: 49497535
 **************************************/

#include <vector>
#include <string>

#ifndef TOKEN_H_
#define TOKEN_H_

using namespace std;

class Token 
{
	public:
		//Different constructors for different purposes
		Token();
		Token(string type, int envNum );                                // for environment
		Token(string value, string type);                               // simple constructor
		Token(string type, int betaIfDeltaNum, int betaElseDeltaNum);
		Token(string type, string lamdaParam, int lamdaNum);            // for lambda closure
		virtual ~Token();
		
		string value;
		string type;
		int lambdaNum;
		string lambdaParam;
		int envNum;
		bool isTuple;
		vector<Token> tuple;
		int tauCount;
		int lambdaEnv;
		void construct();
		int betaIfDeltaNum;
		int betaElseDeltaNum;		
};

#endif
