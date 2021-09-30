/*
 * Core.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */
#include "Core.h"
#include <string>
#include <iostream>
#include "ReadVariant.h"
#include "Reader.h"

/**
 * @brief Construct a new Core object. 
 *
 * @param ref Reference needed for comparing and reporting variants. 
 *
 * Only thing to initialize is reference, hasher initializes itself automatically.
 * 
 */
Core::Core(Reference *ref, Reader *reader)
{
	reference = ref;
	this->reader = reader;
}

/**
 * @brief Destroy the Core object
 * 
 * There is nothing to be destroyed, everything that needs to be destroyed gets freed while running.
 * 
 */
Core::~Core()
{
}

/**
 * @brief Find all variants in given Read.
 * 
 *
 * This method is the bulk of analysis.
 * It goes through every base and its corresponding CIGAR string, and creates 
 * linked list of variants that get returned.
 * It doesn't have to deal with any garbage collection,
 * because it gives pointer to the linked list to the Read, which disposes
 * of it when it is destructed.
 * 
 * @param read Read to be analyzed.
 * @return Linked list of ReadVariant-s or nullptr when none found.
 * 
 * @todo This method needs to be split into more smaller methods for expandability,
 * i.e. split every "big" case into its own method, add "pre-case" method,
 * which will keep track of every base etc.
 */
ReadVariant *Core::analyzeRead(Read *read)
{
	if (read == nullptr)
	{ // This will happen for example when there is no pair read.
		return nullptr;
	}


	ReadVariant *first = new ReadVariant(); // This is the head of a linked list.
	ReadVariant *last = first;
	unsigned int referenceIndex = reference->getIndex(read->rname, read->pos);
	unsigned int referenceOffset = referenceIndex;

	string insertionString; // Needed for solving insertions
	size_t readIndex = 0;
	size_t remainingCigar = read->nextCigar();
	if (read->mapq >= reference->minMapQ)
	{	
		while (remainingCigar && referenceIndex < reference->length)
		{ // This will stop when there is no more cigar string to read
			// i.e. no more read sequence.
			if (char2Fred(read->qual[readIndex]) >= reference->minQual)
			{			
				string location = read->rname + '\t' + to_string(read->pos + referenceIndex - referenceOffset);
				switch (read->cigarType)
				{
				case cigarState::M:	 // This is easier and prettier than to create more cases.
				case cigarState::EQ: // But that would be definitely faster, the question is how much.
				case cigarState::X:	 // I would guess that quite a bit, if =/X is used, but that is very rarely...	
					reference->addTotalDepth(referenceIndex);
					reference->addQTotalDepth(referenceIndex);
					if (reference->ref[referenceIndex] == read->seq[readIndex])
					{
					}

					else
					{
						last->next = new ReadVariant(referenceIndex,
													string(1, read->seq[readIndex]),
													variantType::SUBSTITUTION, location);
						last = last->next;
					}

					++readIndex;
					++referenceIndex;
					break;

				case cigarState::I:
					location = read->rname + '\t' + to_string(read->pos + referenceIndex - referenceOffset - 1);
					insertionString = "";
					if (readIndex > 0) // This should never happen, because first a match should be found...
					{				   // But this way it will not crash the program.
						insertionString.push_back(read->seq[readIndex - 1]);
					}

					insertionString.push_back(read->seq[readIndex]);
					++readIndex;
					while (remainingCigar != 1 && referenceIndex < reference->length) //Read all but the last one, next Cigar will be new
					{
						remainingCigar = read->nextCigar();
						insertionString.push_back(read->seq[readIndex]);
						++readIndex;
					}
					last->next = new ReadVariant(referenceIndex - 1, insertionString,
												variantType::INSERTION, location);
					last = last->next;
					++readIndex;
					break;

				case cigarState::D:
					last->next = new ReadVariant(referenceIndex, ".",
												variantType::DELETION, location);
					last = last->next;
					++referenceIndex;
					break;

				case cigarState::N:
					++referenceIndex;
					break;

				case cigarState::S:
					++readIndex;
					break;

				case cigarState::H:
				case cigarState::P:
					break;

				default:
					break;
				}
			}
			else cerr << "THIS is wrong!!\n";
			remainingCigar = read->nextCigar();
		}
	}
	else
	{
		while (remainingCigar && referenceIndex < reference->length)
		{
			switch (read->cigarType)
			{
			case cigarState::M:	 // This is easier and prettier than to create more cases.
			case cigarState::EQ: // But that would be definitely faster, the question is how much.
			case cigarState::X:	 // I would guess that quite a bit, if =/X is used, but that is very rarely...	
				reference->addTotalDepth(referenceIndex);
				++readIndex;
				++referenceIndex;
				break;

			case cigarState::I:
				++readIndex;
				while (remainingCigar != 1 && referenceIndex < reference->length) //Read all but the last one, next Cigar will be new
				{
					remainingCigar = read->nextCigar();
					++readIndex;
				}
				++readIndex;
				break;

			case cigarState::D:
			case cigarState::N:
				++referenceIndex;
				break;

			case cigarState::S:
				++readIndex;
				break;

			case cigarState::H:
			case cigarState::P:
				break;

			default:
				break;
			}
			remainingCigar = read->nextCigar();
		}
	}
	
	read->variants = first;
	return first->next;
}

/**
 * @brief Return hash for given variant to be put into Reference::reportVariant.
 * 
 * @param variant ReadVariant to be hashed.
 * @return Requested hash for Reference.
 * This is just a standard library implementation.
 * It is not guaranteed to give O(1) complexity to the hashmap,
 * it will need to be redone right.
 * But it works and that's what matters. 
 * 
 * @todo Create own hasher, which will guarantee O(1) complexity to the 
 * Reference hashmap.
 * 
 * @todo Move this to Reference for paralelization.
 */
unsigned long Core::Variant2int(ReadVariant *variant)
{
	unsigned long ret;
	ret = reference->hasher(variant->bases);
	ret << (sizeof(ret) * 8 / 2);
	ret += variant->index;
	return ret;
}

/**
 * @brief Report variants, when two variants on the same index are found. 
 * 
 * Just dump everything with the same index into this method
 * and it distributes it lower, where everything should be.
 * Just make sure that the variants are not nullptr.
 * 
 * @param first The first Read from the pair. 
 * @param second The second Read from the pair. 
 * @param firstRV ReadVariant asociated to the first read. 
 * @param secondRV ReadVariant asociated to the second read. 
 * 
 * @throws invalid_argument When at least one of the arguments is nullptr.
 */
void Core::reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
							 ReadVariant *secondRV)
{
	if (first == nullptr || second == nullptr ||
		firstRV == nullptr || secondRV == nullptr)
	{
		throw invalid_argument("nullptr given as a ReadVariant!");
	}

	char refBase = reference->ref[firstRV->index];
	unsigned long firstHash = Variant2int(firstRV);
	unsigned long secondHash = Variant2int(secondRV);
	if (firstHash == secondHash)
	{
		reference->reportVariant(firstHash, false, false, true, firstRV,
								 refBase);
	}
	else
	{
		reportFirstReadVariant(first, firstRV);
		reportSecondReadVariant(second, secondRV);
	}
}

/**
 * @brief Report variant from the first read in pair. 
 * 
 * This just passes the responsibility to Reference, which then takes care of everything.
 * 
 * @param first Read to which the variant is associated. 
 * @param firstRV ReadVariant to be reported. 
 */
void Core::reportFirstReadVariant(Read *first, ReadVariant *firstRV)
{
	char refBase = reference->ref[firstRV->index];
	unsigned long firstHash = Variant2int(firstRV);

	reference->reportVariant(firstHash, true, false, false, firstRV, refBase);
}

/**
 * @brief Report variant from the second read in pair.
 * 
 * This just passes the responsibility to Reference, which then takes care of everything.
 * 
 * @param second Read to which the variant is associated. 
 * @param secondRV eadVariant to be reported. 
 * 
 * @todo This is almost a duplicate method of Core::reportFirstReadVariant,
 * they could very well be written into just one method.
 */
void Core::reportSecondReadVariant(Read *second, ReadVariant *secondRV)
{
	char refBase = reference->ref[secondRV->index];
	unsigned long secondHash = Variant2int(secondRV);
	reference->reportVariant(secondHash, false, true, false, secondRV, refBase);
}

/**
 * @brief Converts base quality from char to size_t
 * 
 * @param char to be evaluated
 * @return size_t 
 */
size_t Core::char2Fred(char inp)
{
	return int(inp)-33;
}

/**
 * @brief Analyze a pair of reads and report their variants, this is THE method to call.
 * 
 * This method uses Core::analyzeRead and Core::ReportReadVariant etc. methods
 * and just serves as a junction for everything.
 * It deletes Reads given, which also deletes all Variants that have originated from them.
 * 
 * @exceptsafe If this throws an exception, something went horribly wrong!
 * 
 * @param first First Read of the pair. Can also be nullptr
 */
void Core::analyzeReads(Read *first)
{
	if (first == nullptr)
	{
		return;
	}
	
	Read *second = first->pair;
	ReadVariant *firstVariant = analyzeRead(first);
	ReadVariant *secondVariant = analyzeRead(second);
	ReadVariant *oldFirstVariant;
	ReadVariant *oldSecondVariant;
	oldFirstVariant = firstVariant;
	oldSecondVariant = secondVariant;
	while (firstVariant != nullptr && secondVariant != nullptr)
	{
		if (firstVariant->index == secondVariant->index)
		{
			oldFirstVariant=firstVariant;
			oldSecondVariant=secondVariant;
			firstVariant = firstVariant->next;
			secondVariant = secondVariant->next;
			reportReadVariant(first, second, oldFirstVariant, oldSecondVariant);
		}
		else if (firstVariant->index < secondVariant->index)
		{
			oldFirstVariant=firstVariant;
			firstVariant = firstVariant->next;
			reportFirstReadVariant(first, oldFirstVariant);
		}
		else
		{
			oldSecondVariant=secondVariant;
			secondVariant = secondVariant->next;
			reportSecondReadVariant(second, oldSecondVariant);
		}
	}
	while (firstVariant != nullptr)
	{
		oldFirstVariant=firstVariant;
		firstVariant = firstVariant->next;
		reportFirstReadVariant(first, oldFirstVariant);
	}
	while (secondVariant != nullptr)
	{
		oldSecondVariant=secondVariant;
		secondVariant = secondVariant->next;
		reportSecondReadVariant(second, oldSecondVariant);
	}
	delete first; //This will automatically delete also second
};

void Core::doAnalysis()
{
	Read *newRead;
	newRead = reader->getPairReads();
	while (newRead != nullptr){        
		analyzeReads(newRead);
		newRead = reader->getPairReads();
	}
	cerr << "This analysis has ended!";
}
void Core::operator()()
{
	doAnalysis();
}