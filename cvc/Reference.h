/*
 * Reference.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include "ReferenceVariant.h"
#include "ReadVariant.h"

using namespace std;
enum class Base {
	G, T, A, C, EMPTY
};

string Base2String(Base*, size_t);
char Base2Char(Base);

class Reference {
public:
	Reference(string);
	~Reference();
	unsigned int getLength();
	vector<char> ref;
	//vector<unsigned int> matchingReads;
	//vector<unsigned int> filteredMatchingReads;
	unsigned int getIndex(string file_name, unsigned int index);
	void reportVariant(unsigned long hash, bool first, bool second, bool pair,
			ReadVariant *variant, char refBase);
	string outputVariants();

private:
	unsigned int length;
	void setLength(ifstream&);
	void createArray(ifstream&);
	char bases[8] = { 'A', 'a', 'G', 'g', 'T', 't', 'C', 'c' };
	void skipLine(ifstream&);
	unordered_map<string, unsigned int> *chromozome_starts;
	unordered_map<unsigned long, ReferenceVariant*> *variants;
};
