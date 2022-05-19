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
	void setReadIndices(Read *first, Read *second);
	void reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
						   ReadVariant *secondRV);

	void reportSingleReadVariant(Read *read, ReadVariant *variant, bool paired);
	void reportFirstReadVariant(Read *first, ReadVariant *firstRV, bool paired);

	void reportSecondReadVariant(Read *second, ReadVariant *secondRV, bool paired);

	unsigned long Variant2int(ReadVariant *variant);

	ReadVariant *analyzeRead(Read *read);

	void solveM(Read *read);

	void solveI(Read *read);

	void solveD(Read *read);

	size_t readIndex;

	size_t remainingCigar;

	unsigned int referenceIndex;

	unsigned int referenceOffset;

	ReadVariant *last;

	void solveMapQPass(Read *read);

	void solveMapQFail(Read *read);

	bool mapQFilter(Read *read);

	bool baseQFilter(Read *read);

public:
	Core(Reference *ref, Reader *reader);

	~Core();

	void operator()();

	void doAnalysis();

	void analyzeReads(Read *first);
};
