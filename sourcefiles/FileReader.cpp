/**********************************
 * FileReader.cpp
 * UFID: 49497535
 **********************************/
 
#include <iostream>
#include <fstream>
#include <string>
#include "FileReader.h"

using namespace std;

FileReader::FileReader(char* fileName) 
{
	this->fileName = fileName;       //set filename
}

FileReader::~FileReader() 
{

}

string FileReader::getFileContent()
{
	string returnString;
	string tempString;
	ifstream inputFile;
	
	inputFile.open(this->fileName, ios::in);
	if(!inputFile.is_open())                                  //input file did not open successfully
	{		
		cout << "Error!: Problem in opening the input file";
	} 
	else
	{
		while(inputFile.good())                               //read the entire file and store it in returnString
		{
			getline(inputFile,tempString);
			returnString += tempString + '\n';
		}
	}	
	return returnString;
}
