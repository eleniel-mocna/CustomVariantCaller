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
	string toTSV();
	void addDP(unsigned int DP);
	void addQDP(unsigned int QDP);
	unsigned int DP;
	unsigned int QDP;
	ReferenceVariant(ReadVariant *variant, string referenceBases);
	unsigned int position;
	string bases;
	string position_name;
	string referenceBases;
	unsigned int firstCountPairUnspanned;
	unsigned int firstCountPairSpanned;
	unsigned int secondCountPairUnspanned;
	unsigned int secondCountPairSpanned;
	unsigned int pairsCount;
};
