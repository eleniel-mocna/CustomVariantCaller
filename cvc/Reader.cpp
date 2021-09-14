#include "Reader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Read.h"
using namespace std;

char HEADER_CHAR = '@';

Reader::Reader(string file_name)
{
	myReadFile.open(file_name);
	open = true;
	line_index = 0;
	skipHeader();
	nextRead = getNewRead();
}

Reader::~Reader()
{
	myReadFile.close();
}

string Reader::getLine()
{
	/* Return the next line or empty string if there are no lines to read.*/
	if (open)
	{
		lastLine = currentLine;
		if (!getline(myReadFile, currentLine, '\n'))
		{
			open = false;
		}

		++line_index;
		return lastLine;
	}
	else
	{
		return "";
	}

}

Read* Reader::getNewRead()
{
	if (open)
	{
		vector<string> splitted = splitString(getLine(), '\t');
		Read* ret = new Read(splitted[0],
							stoi(splitted[1]),
							splitted[2],
							stoi(splitted[3]),
							stoi(splitted[4]),
							splitted[5],
							splitted[6],
							stoi(splitted[7]),
							stoi(splitted[8]),
							splitted[9],
							splitted[10]);
		if (ret->cigar=="*")
		{
			delete ret;
			return getNewRead();
		}
		else
		{
			return ret;
		}
	}
	return nullptr;
	
	
}

vector<string> Reader::splitString(string input, char delimeter)
{
	vector<string> ret;
	string current = "";
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input[i]==delimeter)
		{
			ret.push_back(current);
			current = "";
		}
		else
		{
			current.push_back(input[i]);
		}
	}
	return ret;
}

void Reader::skipHeader()
{
	do
	{
		getline(myReadFile, currentLine);
	} while (currentLine[0] == HEADER_CHAR);
}

Read* Reader::getPairReads()
{
	Read* first = nextRead;
	Read* second = getNewRead();
	if (first->qname == second->qname) //Next read is this ones pair
	{
		(*first).setPair(second);
		nextRead = getNewRead();
		return first;
	}
	else //We don't have a pair
	{
		nextRead=second;
		return first;
	}
}