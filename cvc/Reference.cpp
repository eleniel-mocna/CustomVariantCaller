/*
 * Reference.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#include <iostream>
#include <fstream>
#include <cassert>
#include "Reference.h"
#include "ReferenceVariant.h"
#include "ReadVariant.h"
#include "Read.h"
#include <stdexcept>

using namespace std;

char Base2Char(Base b) {
	switch (b) {
	case Base::A:
		return 'A';
		break;
	case Base::G:
		return 'G';
		break;
	case Base::T:
		return 'T';
		break;
	case Base::C:
		return 'C';
		break;
	default:
		return '#';
		break;
	}
}

string Base2String(Base *b, size_t length) {
	string ret = "";
	for (size_t i = 0; i < length; i++) {
		ret.push_back(Base2Char(b[i]));
	}
	return ret;

}

void Reference::setLength(ifstream &file) {
	length = 1; //indexing is 1-based
	string line;
	chromozome_starts = new unordered_map<string, unsigned int>();
	while (getline(file, line)) {
		if (line[0] == '@' || line[0] == ';' || line[0] == '>') //This line is a header or comment
				{
			line.erase(0, 1);
			cerr << line + ": " + to_string(length - 1) + '\n';
			(*chromozome_starts)[line] = length - 1;
		} else {
			for (char character : line) {
				if (isalpha(character)) {
					++length;
				}
			}
		}
	}
}

void Reference::skipLine(ifstream &file) {
	while (file.get() != '\n')
		;
}

void Reference::createArray(ifstream &file) {
	file.clear();
	file.seekg(0);
	ref = *(new vector<char>(length));
	matchingReads = *(new vector<unsigned int>(length, 0));
	//filteredMatchingReads = *(new vector<unsigned int>(length, 0));
	string line;
	unsigned int i = 1; //indexing is 1-based

	while (getline(file, line)) // We need to read this line by line, because of comments
	{
		if (line[0] == '@' || line[0] == ';' || line[0] == '>') //This line is a header or comment
				{
		} else {
			for (char &last : line) {
				if (isalpha(last)) {
					ref[i] = toupper(last);
					++i;
					if (i % 100000000 == 0) {
						cerr
								<< "Loaded base " + to_string(i) + "/"
										+ to_string(length) + "...\n";
					}

				}
			}
		}
	}
	assert(length == i && "Input file wasn't read correctly!");
}

unsigned int Reference::getIndex(string file, unsigned int index) {
	return (*chromozome_starts)[file] + index;
}

Reference::Reference(string path) {
	cerr << "Loading reference from storage...\n";
	ifstream file(path);
	variants = new unordered_map<unsigned long, ReferenceVariant*>;
	cerr << "Reference loaded!\nSetting length...\n";
	setLength(file);
	cerr << "Length set: " + to_string(length) + "\nCreating array...\n";
	createArray(file);
	cerr << "Array created, reference initialized!\n";
}

Reference::~Reference() {
	//delete &ref;
}

unsigned int Reference::getLength() {
	return length;
}

void Reference::reportVariant(unsigned long hash, bool first, bool second,
		bool pair, ReadVariant *variant, char refBase) {
	ReferenceVariant *refVar;
	variants->end();
	if (variants->find(hash) == variants->end()) {
		refVar = new ReferenceVariant(variant, refBase);
		(*variants)[hash] = refVar;
	} else {
		refVar = (*variants)[hash];
	}
	refVar->firstCount += first;
	refVar->secondCount += second;
	refVar->pairsCount += pair;

}

string Reference::outputVariants() {

	string ret = "";
	for (auto varIter = variants->begin(); varIter != variants->end();
			varIter++) {
		ReferenceVariant *var = varIter->second;
		var->addDP(matchingReads[var->position]);
		ret += var->toString();
		ret += '\n';
	}
	return ret;
}
