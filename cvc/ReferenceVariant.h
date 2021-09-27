/*
 * ReferenceVariant.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include <string>
#include "ReadVariant.h"

using namespace std;
/**
 * @brief This structure holds all information about one variant, stored in Reference class.
 * 
 * This is only a structure which is used by Reference::reportVariant method.
 * It just holds information which can be output by ReferenceVariant::toString
 * as one vcf file line.
 * 
 */
struct ReferenceVariant {
	string toString();
	void addDP(unsigned int DP);
	void addQDP(unsigned int QDP);
	unsigned int DP;
	unsigned int QDP;
	ReferenceVariant(ReadVariant *variant, char referenceBase);
	unsigned int position;
	string bases;
	string position_name;
	char referenceBase;
	unsigned int firstCount;
	unsigned int secondCount;
	unsigned int pairsCount;
};
