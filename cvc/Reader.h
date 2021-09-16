/*
 * Reader.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include "Read.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Reader {
public:
	Reader(string);
	~Reader();
	string getLine();

	Read* getPairReads();
	bool open;

private:
	Read* getNewRead();
	void skipHeader();
	vector<string> splitString(string, char);
	ifstream myReadFile;
	string currentLine;
	string lastLine;
	Read *nextRead = nullptr;
	unsigned int line_index;
};
