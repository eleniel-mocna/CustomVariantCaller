#include "Read.h"
#include <stdexcept>
#include <cassert>
#include <iostream>

using namespace std;

cigarState char2cigarState(char inp)
{
	switch (inp)
	{
	case 'M': case 'm':
		return cigarState::M;
		break;
	case 'I': case 'i':
		return cigarState::I;
		break;
	case 'D': case 'd':
		return cigarState::D;
		break;
	case 'N': case 'n':
		return cigarState::N;
		break;
	case 'S': case 's':
		return cigarState::S;
		break;
	case 'H': case 'h':
		return cigarState::H;
		break;				
	case 'P': case 'p':
		return cigarState::P;
		break;
	case '=':
		return cigarState::EQ;
		break;		
	case 'X': case 'x':
		return cigarState::X;
	default:
		throw invalid_argument("Received non-valid cigarState character!");
	}
}

char cigarState2char(cigarState inp)
{
	switch (inp)
	{
	case cigarState::M:
		return 'M';
		break;
	case cigarState::I:
		return 'I';
		break;
	case cigarState::D:
		return 'D';
		break;
	case cigarState::N:
		return 'N';
		break;
	case cigarState::S:
		return 'S';
		break;
	case cigarState::H:
		return 'H';
		break;
	case cigarState::P:
		return 'P';
		break;		
	case cigarState::EQ:
		return '=';
		break;
	case cigarState::X:
		return 'X';
		break;
	default:
		throw invalid_argument("There are more cigar states than this? Ooops...");
	}
}

void Read::configure(size_t offset)
{
	return;
}

Read::Read()
{}

Read::Read(string qname,
	size_t flag,
	string rname,
	unsigned int pos,
	size_t mapq,
	string cigar,
	string rnext,
	unsigned int pnext,
	int tlen,
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
	this->tlen = abs(tlen);
	this->seq = seq;
	this->qual = qual;
	pair = nullptr;
	cigarIndex = 0;
	remainingThisCigar = 1; // We'll subtract 1 on first call of nextCigar
}

Read::~Read()
{
	delete pair;
}

size_t Read::nextCigar()
{	
	if (remainingThisCigar==0) // We overran the cigar string
	{
		return 0;
	}
	
	--remainingThisCigar;
	if (remainingThisCigar==0)
	{
		if (cigarIndex<cigar.length())
		{
			cout << cigar.length();
			setCigarLength();
			setCigarType();
		}
		else
		{			
			remainingThisCigar=0;
		}
	}
	return remainingThisCigar;
}
void Read::setCigarLength()
{
	assert(remainingThisCigar==0 && "setCigarLength called when there was unused cigar left");
	remainingThisCigar = 0;
	while (isdigit(cigar[cigarIndex]))
	{
		remainingThisCigar *= 10;
		remainingThisCigar += cigar[cigarIndex]-48;
		++cigarIndex;
	}
	
}
void Read::setCigarType()
{
	cigarType = char2cigarState(cigar[cigarIndex]);
	++cigarIndex;
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
