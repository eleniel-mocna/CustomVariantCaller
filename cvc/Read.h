#pragma once

using namespace std;
#include <string>;
class Read
{
public:
	Read(string, size_t, string, unsigned int, size_t, string, string, unsigned int, unsigned int, string, string);
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
};

