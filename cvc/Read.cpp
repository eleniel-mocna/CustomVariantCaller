#include "Read.h"

Read::Read(string qname,
	size_t flag,
	string rname,
	unsigned int pos,
	size_t mapq,
	string cigar,
	string rnext,
	unsigned int pnext,
	unsigned int tlen,
	string seq,
	string qual)
{
	this->qname = qname;
	this->flag = flag;
	this->rname = rname;
	this->pos = pos;
	this->mapq = mapq;
	this->cigar = cigar;
	this->rnext = rnext;
	this->pnext = pnext;
	this->tlen = tlen;
	this->seq = seq;
	this->qual = qual;
}
