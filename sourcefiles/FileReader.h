/*************************************
 * FileReader.h
 * UFID: 49497535
 *************************************/

#ifndef FILEREADER_H_
#define FILEREADER_H_

using namespace std;

class FileReader 
{
	private:
		char* fileName;
	
	public:
		FileReader(char* fileName);
		virtual ~FileReader();
		
		string getFileContent();
};

#endif 
