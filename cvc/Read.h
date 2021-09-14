#pragma once

#include <string>
#include "Reference.h"
using namespace std;

enum class cigarState
{
	M, // Match/Mismatch
	I, // Insertion
	D, // Deletion
	N, // Skipped region from reference
	S, // Soft clipping
	H, // Hard clipping
	P, // padding
	EQ, // Match
	X // Mismatch
};

cigarState char2cigarState(char);
char cigarState2char(cigarState);

class Read
{
private:
	int currentOffset;
	bool readable=false;
	size_t cigarIndex;
	void setCigarLength();
	void setCigarType();


public:
	Read();
	~Read();
	size_t remainingThisCigar; //goto private
	Read(string, size_t, string, unsigned int, size_t, string, string, unsigned int, int, string, string);
	void setPair(Read*);
	size_t nextCigar();
	cigarState cigarType;
	string qname;
	size_t flag;
	string rname;
	unsigned int pos;
	size_t mapq;
	string cigar;
	string rnext;
	unsigned int pnext;
	unsigned int tlen;
	string seq;
	string qual;
	Read* pair;
	string toString();
	void configure(size_t offset); // TODO
	Base nextBase(); // TODO
};

