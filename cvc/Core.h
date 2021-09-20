/*
 * Core.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */

#pragma once

#include "Read.h"
#include "Reader.h"
#include "Reference.h"
#include "ReadVariant.h"
#include <string>
#include <functional>
using namespace std;


/**
 * @brief This class takes care of analysis of individual pairs of reads.
 * 
 * Everything should be expandable for paralelization purpouses, because this
 * is the bottleneck of this program.
 * 
 */
class Core
{
private:
	Reference *reference;
	void reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
						   ReadVariant *secondRV);

	void reportFirstReadVariant(Read *first, ReadVariant *firstRV);
	
	void reportSecondReadVariant(Read *second, ReadVariant *secondRV);
	
	unsigned long Variant2int(ReadVariant *variant);

	ReadVariant *analyzeRead(Read *read);

	hash<std::string> hasher; // TODO: This needs to moved to Reference for parallelisation!

public:
	Core(Reference *ref);

	~Core();

	void analyzeReads(Read *first);
};
