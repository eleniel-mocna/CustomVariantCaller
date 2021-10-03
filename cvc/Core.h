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

	static size_t char2Fred(char);

	void solveM(Read *read);

	void solveI(Read *read);

	void solveD(Read *read);

	size_t readIndex;

	size_t remainingCigar;

	unsigned int referenceIndex;

	unsigned int referenceOffset;

	ReadVariant *last;

public:
	Core(Reference *ref, Reader *reader);

	~Core();

	void operator()();

	void doAnalysis();

	void analyzeReads(Read *first);
};
