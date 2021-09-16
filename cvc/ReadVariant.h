/*
 * ReadVariant.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include <string>

using namespace std;

enum class variantType {
	DELETION, INSETION, SUBSTITUTION
};

string variantType2string(variantType type);

struct ReadVariant {
	ReadVariant();
	ReadVariant(unsigned int position, string bases, variantType type,
			string location);
	~ReadVariant();
	string toString();
	unsigned int index;
	variantType type;
	string bases;
	string location;
	ReadVariant *next;
};
