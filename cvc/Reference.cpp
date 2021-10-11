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

/**
 * @brief Read through the whole reference and set the length of it.
 * 
 * This also sets the chromozome starts.
 * The file needs to be opened and sought to beggining, after this call
 * the file will be sought to the end.
 * Also, this shall be called only from the constructor.
 * 
 * @note An argument could be made not to read the whole reference,
 * because of the time saving, but this is more defensive and will work
 * on every type of fasta file.
 * 
 * @param file Opened file with the reference
 */
void Reference::setLength(ifstream &file)
{
	length = 1; //indexing is 1-based
	string line;
	chromozome_starts = new unordered_map<string, unsigned int>();
	while (getline(file, line))
	{
		if (line[0] == '@' || line[0] == ';' || line[0] == '>') //This line is a header or comment
		{
			line.erase(0, 1);
			cerr << line + ": " + to_string(length - 1) + '\n';
			(*chromozome_starts)[line] = length - 1;
		}
		else
		{
			for (char character : line)
			{
				if (isalpha(character))
				{
					++length;
				}
			}
		}
	}
}

/**
 * @brief Fill the reference array with bases and set totalDepths to 0.
 * 
 * This method goes through the whole array and fills it with bases
 * as standardized characters (uppercase).
 * 
 * @throw assert when the file doesn't have the same length
 * 	as determined by Reference::setLength
 * 
 * @param file 
 */
void Reference::createArray(ifstream &file)
{
	file.clear();
	file.seekg(0);
	ref = *(new vector<char>(length));
	//filteredMatchingReads = *(new vector<unsigned int>(length, 0));
	string line;
	unsigned int i = 1; //indexing is 1-based

	while (getline(file, line)) // We need to read this line by line, because of comments
	{
		if (line[0] == '@' || line[0] == ';' || line[0] == '>') //This line is a header or comment
		{
		}
		else
		{
			for (char &last : line)
			{
				if (isalpha(last))
				{
					ref[i] = toupper(last);
					++i;
					if (i % 100000000 == 0)
					{
						cerr
							<< "Loaded base " + to_string(i) + "/" + to_string(length) + "...\n";
					}
				}
			}
		}
	}
	assert(length == i && "Input file wasn't read correctly!");
}

/**
 * @brief Get array index for human-readable index in the reference.
 * 
 * @param file In which file(=chromozome).
 * @param index Which index in said file(=chromozome).
 * @return unsigned int 
 */
unsigned int Reference::getIndex(string file, unsigned int index)
{
	return (*chromozome_starts)[file] + index;
}

/**
 * @brief Construct a new Reference object
 * 
 * This reads the whole file and prepares everything.
 * 
 * @param path 
 */
Reference::Reference(string path, size_t minMapQ, size_t minQual)
{
	this->minMapQ = minMapQ;
	this->minQual = minQual;
	cerr << "Loading reference from storage...\n";
	ifstream file(path);
	variants = new unordered_map<unsigned long, ReferenceVariant *>;
	totalDepth = new unordered_map<unsigned int, unsigned int>;
	qTotalDepth = new unordered_map<unsigned int, unsigned int>;
	cerr << "Reference loaded!\nSetting length...\n";
	setLength(file);
	cerr << "Length set: " + to_string(length) + "\nCreating array...\n";
	createArray(file);
	cerr << "Array created, reference initialized!\n";
	file.close();
}

/**
 * @brief Destroy the Reference:: Reference object
 * 
 */
Reference::~Reference()
{
	delete &ref;
}

/**
 * @brief Return the length of the reference
 * 
 * @return unsigned int 
 */
unsigned int Reference::getLength()
{
	return length;
}

/**
 * @brief Save given variant.
 * 
 * @param hash Hash generated for this variant and hasmap.
 * @param first Is this variant supported only by the first Read?
 * @param second Is this variant supported only by the second Read?
 * @param pair Is this variant supported by both reads in the pair?
 * @param variant What variant is reported.
 * @param refBase What is the base in this variant's location in the reference.
 * 
 * @throws invalid_argument given variant is of unknown type
 */
void Reference::reportVariant(unsigned long hash, bool first, bool second,
							  bool pair, ReadVariant *variant)
{
	ReferenceVariant *refVar;
	string refBases = "";
	switch (variant->type)
	{
	case variantType::INSERTION:
	case variantType::SUBSTITUTION:
		refBases.push_back(ref[variant->index]);
		break;

	case variantType::DELETION:
		for (size_t i = 0; i < variant->bases.length() + 1; i++)
		{
			refBases.push_back(ref[variant->index + i]);
		}
		break;

	default:
		cerr << variant->toString();
		throw invalid_argument("Received variant with unexpected variantType!");
	}
	variantLock.lock();
	if (variants->find(hash) == variants->end())
	{
		refVar = new ReferenceVariant(variant, refBases);
		(*variants)[hash] = refVar;
	}
	else
	{
		refVar = (*variants)[hash];
	}
	refVar->firstCount += first;
	refVar->secondCount += second;
	refVar->pairsCount += pair;
	variantLock.unlock();
}

/**
 * @brief Return string as a vcf file of found variants.
 * 
 * @return string 
 */

void Reference::AddToMap(unsigned int index, unordered_map<unsigned int, unsigned int> *map)
{
	if (map->find(index) == map->end())
	{
		(*map)[index] = 1;
	}
	else
	{
		(*map)[index] += 1;
	}
}

void Reference::addTotalDepth(unsigned int index)
{
	totalDepthLock.lock();
	AddToMap(index, totalDepth);
	totalDepthLock.unlock();
}

void Reference::addQTotalDepth(unsigned int index)
{
	qTotalDepthLock.lock();
	AddToMap(index, qTotalDepth);
	qTotalDepthLock.unlock();
}

unsigned int Reference::getFromMap(unsigned int index, unordered_map<unsigned int, unsigned int> *map)
{
	if (map->find(index) == map->end())
	{
		return 0;
	}
	else
	{
		return (*map)[index];
	}
}

unsigned int Reference::getTotalDepth(unsigned int index)
{
	return getFromMap(index, totalDepth);
}

unsigned int Reference::getQTotalDepth(unsigned int index)
{
	return getFromMap(index, qTotalDepth);
}