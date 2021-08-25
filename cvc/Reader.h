#pragma once

#include "Reader.h";
using namespace std;

class Reader
{
public:
	Reader(string);
	~Reader();
	string getLine();
	Read getRead();
	bool open;

private:
	void skipHeader();
	vector<string> splitString(string, char);
	ifstream myReadFile;
	string currentLine;
	string lastLine;
	unsigned int line_index;
};