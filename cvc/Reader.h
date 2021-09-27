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
#include <mutex>

using namespace std;
/**
 * @brief This is the class that reads the SAM file and produces pair reads.
 * 
 * The main method is Reader::getPairReads, where Read class starts its lifecycle
 * or nullptr when there are no more reads left.
 * 
 * @todo Make paralelization checks
 * 
 */
class Reader {
public:
	Reader(string);
	~Reader();
	Read* getPairReads();

private:
	mutex getLock;
	string getLine();
	Read* getNewRead();
	void skipHeader();
	vector<string> splitString(string, char);
	ifstream myReadFile;
	string currentLine;
	string lastLine;
	Read *nextRead = nullptr;
	unsigned int line_index;
	bool open;
};
