#pragma once

#include <string>
#include "Reference.h"
using namespace std;

enum class cigarState
{MATCH, INSERTION, DELETION};

class Read
{
private:
	int currentOffset;
	bool readable=false;
	
	size_t cigarStep;
	size_t cigarIndex;

public:
	Read();
	Read(string, size_t, string, unsigned int, size_t, string, string, unsigned int, unsigned int, string, string);
	void setPair(Read*);
	string qname;
	size_t flag;
	string rname;
	int pos;
	size_t mapq;
	string cigar;
	string rnext;
	int pnext;
	int tlen;
	string seq;
	string qual;
	Read* pair;
	string toString();
	void configure(size_t offset); // TODO
	Base nextBase(); // TODO
};

