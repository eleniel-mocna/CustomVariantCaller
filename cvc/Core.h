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
	Reader *reader;
	Reference *reference;
	void reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
						   ReadVariant *secondRV);

	void reportFirstReadVariant(Read *first, ReadVariant *firstRV);
	
	void reportSecondReadVariant(Read *second, ReadVariant *secondRV);
	
	unsigned long Variant2int(ReadVariant *variant);

	ReadVariant *analyzeRead(Read *read);

	size_t char2Fred(char);

	 // TODO: This needs to moved to Reference for parallelisation!

public:
	Core(Reference *ref, Reader *reader);

	~Core();

	void operator()();

	void doAnalysis();

	void analyzeReads(Read *first);
};
