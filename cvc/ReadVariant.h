/*
 * ReadVariant.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include <string>

using namespace std;
/**
 * @brief enum class describing the variant type in ReadVariant and RferenceVariant.
 * 
 */
enum class variantType {
	DELETION, INSERTION, SUBSTITUTION
};
/**
 * @brief Simple enum to string conversion
 * 
 * @param type variantType to be conversed
 * @return type as a string.
 */
string variantType2string(variantType type);
/**
 * @brief This struct holds information for a variant in a Read.
 * 
 * Its lifecycle begins in Core::analyzeRead, where it is given to Read class,
 * which takes care of destruction.
 * It forms a linked list via ReadVariant::next, where empty ReadVariant
 * is the head of said linked list and nullptr the end.
 * 
 */
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
