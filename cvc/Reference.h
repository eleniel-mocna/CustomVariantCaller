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

/**
 * @brief Class for get information from the reference and storing variants.
 * 
 * It allows easy access to requested positions in the genome via Reference::ref 
 * and Reference::getIndex, then using Reference::reportVariant it takes care
 * of holding all the variants, which can be then output
 * via Reference::outputVariants.
 * 
 */
class Reference {
public:
	Reference(string);
	~Reference();
	unsigned int getLength();
	/// Array of all bases in the reference.
	///
	vector<char> ref;
	/// Number of Reads covering said base.
	///
	vector<unsigned int> totalDepth;
	unsigned int getIndex(string file_name, unsigned int index);
	void reportVariant(unsigned long hash, bool first, bool second, bool pair,
			ReadVariant *variant, char refBase);
	string outputVariants();
	unsigned int length;

private:
	void setLength(ifstream&);
	void createArray(ifstream&);
	char bases[8] = { 'A', 'a', 'G', 'g', 'T', 't', 'C', 'c' };
	unordered_map<string, unsigned int> *chromozome_starts;
	unordered_map<unsigned long, ReferenceVariant*> *variants;
};
