/*
 * ReadVariant.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#include "ReadVariant.h"
#include <string>
#include <iostream>
/**
 * @brief 
 * 
 * @param type 
 * @return string 
 */
string variantType2string(variantType type) {
	switch (type) {
	case variantType::DELETION:
		return "deletion";
		break;
	case variantType::INSERTION:
		return "insertion";
		break;
	case variantType::SUBSTITUTION:
		return "substitution";
		break;
	default:
		cerr << "Unknown\n";
		return "unknown";
	}
}
/**
 * @brief Construct an empty ReadVariant object, used as the head of its linked list.
 * 
 * This constructor should never be called for a real variant.
 * It is a head of a linked list not contianing any information at all.
 * We can check if a ReadVariant object is just a head by ReadVariant::index==0,
 * because reference indexing is 1-based.
 * 
 */
ReadVariant::ReadVariant() //This shall be called only as the head of a linked list
{                        // This ain't no pretty, but, ya'll, it's pretty short!
	index = 0; // If we created a child class, its destructor would call this ones no matter what...
	bases = "";
	type = variantType::DELETION;
	next = nullptr;
	location="";
}

/**
 * @brief Construct a new ReadVariant object.
 * 
 * This constructs a new ReadVariant with information inside of it.
 * 
 * @param index 1-based index in the reference.
 * @param bases String of bases in this variant ("." for Deletion).
 * @param type Enum type of the variant.
 * @param loc Human-readable location separated by '\t' (e.g. "chr 17\t7775438")
 */
ReadVariant::ReadVariant(unsigned int index, string bases, variantType type,
		string loc) {
	this->index = index;
	this->bases = bases;
	this->type = type;
	this->location = loc;
	next = nullptr;
}

/**
 * @brief Human readable conversion to a pretty string
 * 
 * @return string
 */
string ReadVariant::toString() {
	return "ReadVariant: " + variantType2string(type) + " @ " + to_string(index)
			+ ", " + bases + ", next: " + to_string(bool(next)) + '\n';
}
/**
 * @brief Destroy the ReadVariant object and the whole linked list.
 * 
 */
ReadVariant::~ReadVariant() {
	delete next;
}
