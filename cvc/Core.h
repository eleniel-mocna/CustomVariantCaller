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

class Core {
private:

	Reference *reference;
	void reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
			ReadVariant *secondRV);
	void reportFirstReadVariant(Read *first, ReadVariant *firstRV);
	void reportSecondReadVariant(Read *first, ReadVariant *secondRV);
	unsigned long Variant2int(ReadVariant *variant);
public:
	Core(Reference*);
	~Core();
	void analyzeReads(Read*);
	ReadVariant* analyzeRead(Read *first); // Goto private
	hash<std::string> hasher;
};

