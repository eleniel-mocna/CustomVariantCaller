/*
 * Read.h
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#pragma once

#include <string>
#include "Reference.h"
#include "ReadVariant.h"
using namespace std;

/**
 * @brief Classification of cigar string states.
 * 
 * This is much better then using just regular chars for safety of code.
 */
enum class cigarState {
	/// Match/Mismatch
	M,
	/// Insertion
	I, 
	/// Deletion
	D, 
	/// Skipped region from reference
	N, 
	/// Soft clipping
	S, 
	/// Hard clipping
	H, 
	/// padding
	P, 
	/// Match
	EQ,
	/// Mismatch 
	X 
};

cigarState char2cigarState(char);
char cigarState2char(cigarState);

/**
 * @brief This class holds all information for a single read.
 * 
 * For extracting all of bases, call Read::nextCigar() before reading the next
 * base. When this method returns 0, all bases have been read (at least,
 * if the read line in the original sam file was correct).
 * This class' objects' lifecycle starts in Reader::getRead and ends 
 * in Core::analyzeReads.
 * 
 * @note There is more information in this class than needed,
 * this is for future-proofing and when all features are added, only the useful
 * once will be kept.
 */
class Read {
private:
	size_t cigarIndex;
	void setCigarLength();
	void setCigarType();
	size_t remainingThisCigar;

public:
	~Read();
	Read(string, size_t, string, unsigned int, size_t, string, string,
			unsigned int, int, string, string);
	void setPair(Read*);
	size_t nextCigar();
	string toString();
	cigarState cigarType;
	/// Query template NAME
	///
	string qname;
	/// bitwise FLAG
	///
	size_t flag;
	/// References sequence NAME
	///
	string rname;
	/// 1-based leftmost mapping POSition from the beggining of rname
	///
	unsigned int pos;
	/// MAPping Quality
	///
	size_t mapq;
	/// CIGAR string
	///
	string cigar;
	/// Ref. name of the mate/next read
	///
	string rnext;
	///@param pnext Position of the mate/next read
	/// 
	unsigned int pnext;
	///@param tlen observed Template LENgth
	///
	unsigned int tlen;
	///@param seq segment SEQuence
	///
	string seq;
	///@param qual ASCII of Phred-scaled base QUALity+33 
	///
	string qual;
	/// Pair read to this one
	///
	Read *pair;
	/// Linked list of all variants of this Read.
	///
	ReadVariant* variants;
};

