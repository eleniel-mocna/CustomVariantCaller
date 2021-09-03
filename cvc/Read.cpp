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
	pair = nullptr;
}

string Read::toString()
{
	string ret = "\nRead: ";
	ret += "\n    qname: " + qname;
	ret += "\n     flag: " + to_string(flag);
	ret += "\n    rname: " + rname;
	ret += "\n      pos: " + to_string(pos);
	ret += "\n     mapq: " + to_string(mapq);
	ret += "\n    cigar: " + cigar;
	ret += "\n    rnext: " + rnext;
	ret += "\n    pnext: " + to_string(pnext);
	ret += "\n    tlen: " + to_string(tlen);
	ret += "\n     seq: " + seq;
	ret += "\n    qual: " + qual;
	ret += "\n    pair: " + to_string(bool(pair));
	ret += "\n";
	return ret;
}

void Read::setPair(Read* new_pair)
{
	pair=new_pair;
}

