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
#include <mutex>
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
	Reference(string, size_t minMapQ, size_t minQual);
	~Reference();
	unsigned int getLength();
	/// Array of all bases in the reference.
	///
	vector<char> ref;
	void addTotalDepth(unsigned int index);
	void addQTotalDepth(unsigned int index);
	unsigned int getIndex(string file_name, unsigned int index);
	void reportVariant(unsigned long hash, bool first, bool second, bool pair,
			ReadVariant *variant, char refBase);
	string outputVariants();
	unsigned int length;
	size_t minMapQ;
	size_t minQual;
	hash<std::string> hasher;

private:
	mutex variantLock;
	mutex totalDepthLock;
	mutex qTotalDepthLock;
	/// Depth of reads that match our quality filter
	///
	unordered_map<unsigned int, unsigned int> *qTotalDepth;
	unordered_map<unsigned int, unsigned int> *totalDepth;
	void setLength(ifstream&);
	void createArray(ifstream&);
	void AddToMap(unsigned int index, unordered_map<unsigned int, unsigned int>* map);
	unsigned int getTotalDepth(unsigned int index);
	unsigned int getQTotalDepth(unsigned int index);
	unsigned int getFromMap(unsigned int index, unordered_map<unsigned int, unsigned int>* map);
	char bases[8] = { 'A', 'a', 'G', 'g', 'T', 't', 'C', 'c' };
	unordered_map<string, unsigned int> *chromozome_starts;
	unordered_map<unsigned long, ReferenceVariant*> *variants;
};
