/*
 * Read.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */

#include "Read.h"
#include <stdexcept>
#include <cassert>
#include <iostream>

using namespace std;

string toBinary(size_t n)
{
	string r;
	for (size_t i = 0; i < 12; i++)
	{
		r = (n % 2 == 0 ? "0" : "1") + r;
		n /= 2;
		if (i == 3 || i == 7)
		{
			r = ' ' + r;
		}
	}
	return r;
}

/**
 * @brief Convert char into a cigarState enum.
 *
 * @param inp Char to be converted
 *
 * @throws invalid_argument When CigarString that is not supported is given (implementation error)
 * @return Resulting cigarState
 */
cigarState char2cigarState(char inp)
{
	switch (inp)
	{
	case 'M':
	case 'm':
		return cigarState::M;
		break;
	case 'I':
	case 'i':
		return cigarState::I;
		break;
	case 'D':
	case 'd':
		return cigarState::D;
		break;
	case 'N':
	case 'n':
		return cigarState::N;
		break;
	case 'S':
	case 's':
		return cigarState::S;
		break;
	case 'H':
	case 'h':
		return cigarState::H;
		break;
	case 'P':
	case 'p':
		return cigarState::P;
		break;
	case '=':
		return cigarState::EQ;
		break;
	case 'X':
	case 'x':
		return cigarState::X;
	default:
		cerr
			<< "ERROR '" + to_string(inp) + "' is not a valid cigarState character!\n";
		throw invalid_argument("Received non-valid cigarState character!");
	}
}

/**
 * @brief Convert cigarState to a char.
 *
 * @param inp cigarState to be converted.
 * @return Converted char.
 */
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
		throw invalid_argument(
			"There are more cigar states than this? Ooops...");
	}
}

/**
 * @brief Construct a new Read object.
 *
 * As this class is mainly for holding data about one read, this constructor
 * takes all information from the sam file and saves it.
 *
 * @param qname Query template NAME
 * @param flag bitwise FLAG
 * @param rname References sequence NAME
 * @param pos 1-based leftmost mapping POSition from the beggining of rname
 * @param mapq MAPping Quality
 * @param cigar CIGAR string
 * @param rnext Ref. name of the mate/next read
 * @param pnext Position of the mate/next read
 * @param tlen observed Template LENgth
 * @param seq segment SEQuence
 * @param qual ASCII of Phred-scaled base QUALity+33
 */
Read::Read(string qname, size_t flag, string rname, unsigned int pos,
		   size_t mapq, string cigar, string rnext, unsigned int pnext, int tlen,
		   string seq, string qual)
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
	this->cigarType = cigarState::M;
	variants = nullptr;
	remainingThisCigar = 1; // We'll subtract 1 on first call of nextCigar
}

/**
 * @brief Destroy the Read object, its pair and their variants.
 *
 * This destructor is called in Core::analyzeReads, where this class' lifecycle
 * ends.
 */
Read::~Read()
{
	delete pair;
	delete variants;
}

/**
 * @brief Move CigarState one base forward.
 *
 * This method shall be called every time before extracting the next base
 * in the sequence. After this call this->cigarType will correspond to
 * the next base's cigar type.
 *
 * @return How many more bases will have the same cigar as the next one.
 */
size_t Read::nextCigar()
{
	if (remainingThisCigar == 0) // We overran the cigar string
	{
		return 0;
	}

	--remainingThisCigar;
	if (remainingThisCigar == 0)
	{
		if (cigarIndex < cigar.length())
		{
			setCigarLength();
			setCigarType();
		}
		else
		{
			remainingThisCigar = 0;
		}
	}
	return remainingThisCigar;
}

/**
 * @brief Call this only from Read::nextCigar, sets the CigarLength.
 *
 */
void Read::setCigarLength()
{
	assert(
		remainingThisCigar == 0 && "setCigarLength called when there was unused cigar left");
	remainingThisCigar = 0;
	while (isdigit(cigar[cigarIndex]))
	{
		remainingThisCigar *= 10;
		remainingThisCigar += cigar[cigarIndex] - 48;
		++cigarIndex;
	}
}

/**
 * @brief Call this only from Read::nextCigar, sets the CigarType.
 *
 */
void Read::setCigarType()
{
	try
	{
		cigarType = char2cigarState(cigar[cigarIndex]);
	}
	catch (const exception &e)
	{
		std::cerr << e.what() << "from: " << cigar << '\n';
		cigarType = cigarState::M;
	}

	++cigarIndex;
}

/**
 * @brief Return nice representation of this Read.
 *
 * @return string
 */
string Read::toString()
{
	string ret = "\nRead: ";
	ret += "\n    qname: " + qname;
	ret += "\n     flag: " + toBinary(flag);
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

/**
 * @brief Set given argument as the pair of this read.
 *
 * @param new_pair Pair of this read.
 */
void Read::setPair(Read *new_pair)
{
	pair = new_pair;
}
/**
 * @brief Does this read span given Reference index?
 *
 * @param index Index in the reference
 * @return true
 * @return false
 */
bool Read::spansPosition(unsigned int index, size_t minBaseQ)
{
	if (!mapQPass ||
		(baseQAtIndex(referenceIndexFromBegin(index)) < minBaseQ))
	{
		return false;
	}
	if (referenceIndexPos == 0)
	{
		throw invalid_argument("Read reference index not set!");
	}
	return index >= referenceIndexPos && index < referenceIndexPos + tlen;
}

/**
 * @brief Set that this read passes the mapQ filter
 *
 */
void Read::passesMapQ()
{
	mapQPass = true;
}

/**
 * @brief Return number of reference bases from begin of this read
 *
 * @param referenceIndex the reference index we want to match
 * @return size_t the number of bases from begin
 */
size_t Read::referenceIndexFromBegin(unsigned int referenceIndex)
{
	size_t ret = referenceIndex - this->referenceIndexPos;
	if (ret > tlen)
	{
		throw invalid_argument("Reference index out of range!");
	}
	return ret;
}

/**
 * @brief Base quality at `indexFromBegin`-th reference base from begin
 * of this read.
 *
 * @param indexFromBegin Number of reference bases from the beginning of this
 * 	read.
 * @return size_t The base quality.
 */
size_t Read::baseQAtIndex(size_t indexFromBegin)
{
	size_t subtraction = 0;
	string numberBuffer = "";
	char cigarType = '.';
	for (auto &&character : cigar)
	{
		if (isdigit(character))
		{
			numberBuffer = numberBuffer + character;
		}
		else
		{
			cigarType = character;
			switch (cigarType)
			{
			case 'M':
			case '=':
			case 'X':
			case 'D':
			case 'N':
				if (indexFromBegin + stoi(numberBuffer) - subtraction < indexFromBegin)
				{
				}
				else
				{
					return char2Fred(cigar[indexFromBegin - subtraction]);
				}
				break;
			case '.':
				throw invalid_argument("Unexpected CIGAR string!");
				break;
			default:
				subtraction = subtraction + stoi(numberBuffer);
				break;
			}
			numberBuffer = "";
		}
	}
	throw invalid_argument("Given index overran the Cigar string @ BaseQAtIndex!");
}

/**
 * @brief Converts base quality from char to size_t
 *
 * @param char to be evaluated
 * @return size_t
 */
size_t Read::char2Fred(char inp)
{
	return int(inp) - 33;
}