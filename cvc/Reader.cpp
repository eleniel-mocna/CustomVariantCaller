/*
 * Reader.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#include "Reader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "Read.h"

using namespace std;

char HEADER_CHAR = '@';

/**
 * @brief Construct a new Reader object
 * 
 * After this construction the object is ready for use,
 * it has skipped the header and primed the Reader::getLine method
 * 
 * @param file_name path to a SAM file to be read or ""/"-" for stdin.
 * 
 * @todo add a check for file not existing
 */
Reader::Reader(string file_name)
{
	this->file_name = file_name;
	load();
}

void Reader::load()
{
	if (file_name=="" || file_name=="-")
	{
		myReadFile = &cin;	
	}
	else
	{
		myReadFile = new ifstream(file_name);
	}
	open = true;
	line_index = 0;
	skipHeader();
	getLine();
	nextRead = getNewRead();
}

/**
 * @brief Destroy the Reader object
 * 
 */
Reader::~Reader()
{
	
}

/**
 * @brief Return the next line as a string.
 * 
 * This is an auxiliary method for Reader::getNewRead,
 * it takes care of EOF error.
 * 
 * @return The next line as a string.
 */
string Reader::getLine()
{
	/* Return the next line or empty string if there are no lines to read.*/
	if (open)
	{
		lastLine = currentLine;
		if (!getline(*myReadFile, currentLine))
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

/**
 * @brief Create a new Read and return the pointer to it.
 * 
 * This is an auxiliary method for Reader::getPairReads,
 * it reads one line and creates a Read from it.
 * When the whole file is read, it returns nullptr.
 * 
 * @return New Read or nullptr
 */
Read *Reader::getNewRead()
{
	if (open)
	{
		vector<string> splitted = splitString(getLine(), '\t');
		Read *ret = new Read(splitted[0], stoi(splitted[1]), splitted[2],
							 stoi(splitted[3]), stoi(splitted[4]), splitted[5], splitted[6],
							 stoi(splitted[7]), stoi(splitted[8]), splitted[9],
							 splitted[10]);
		if (ret->cigar == "*")
		{
			// cerr << "ASTERISK FOUND!\n";
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

/**
 * @brief Splits string to be given to new Read().
 * 
 * This is an auxiliary method for Reader::getNewRead.
 * It really just splits the string by delimeters...
 * 
 * @param input The string to be split.
 * @param delimeter Character by which the input string is to be split
 * @return Vector<string> of split strings.
 */
vector<string> Reader::splitString(string input, char delimeter)
{
	vector<string> ret;
	string current = "";
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input[i] == delimeter)
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
/**
 * @brief Skips header, called from constructor.
 * 
 * Skips the whole header or 1 not commented line.
 * This method should never be called outside of the constructor.
 * 
 */
void Reader::skipHeader()
{
	do
	{
		getline(*myReadFile, currentLine);
	} while (currentLine[0] == HEADER_CHAR);
}

/**
 * @brief Get 2 Reads in a pair or 1 Read, when there is no pair or nullptr.
 * 
 * This is the main method of this class, call it every time you need a new 
 * pair of Reads.
 * It will return a pointer to the first read in the pair, which has a pointer to
 * the second Read set as Read::pair.
 * 
 * @return Pointer to the first Read in a pair.
 */
Read *Reader::getPairReads()
{
	getLock.lock();
	Read *first = nextRead;
	Read *second = getNewRead();
	if (first == nullptr || second == nullptr)
	{
		nextRead = getNewRead();
		getLock.unlock();
		return first; // If first is nullptr than this return nullptr, which is what we want
					  // If second is nullptr and first not, we return first, which is what we want
	}
	if (first->qname == second->qname) //Next read is this ones pair
	{
		(*first).setPair(second);
		nextRead = getNewRead();
		getLock.unlock();
		return first;
	}
	else //We don't have a pair
	{
		nextRead = second;
		getLock.unlock();
		return first;
	}
}
