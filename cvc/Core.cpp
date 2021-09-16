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

Core::Core(Reference *ref) {
	reference = ref;
}

Core::~Core() {
}

ReadVariant* Core::analyzeRead(Read *read) {
	if (read == nullptr) {
		return nullptr;
	}

	ReadVariant *first = new ReadVariant(); // This makes the code cleaner later in this function :-)
	ReadVariant *last = first;
	unsigned int referenceIndex = reference->getIndex(read->rname, read->pos);
	unsigned int referenceOffset = referenceIndex;

	string insertionString;
	size_t readIndex = 0;
	while (read->nextCigar()) {
		string location = read->rname + '\t'
				+ to_string(referenceIndex - referenceOffset);
		switch (read->cigarType) {
		case cigarState::M:
		case cigarState::EQ:
		case cigarState::X:

			if (reference->ref[referenceIndex] == read->seq[readIndex]) {
			} else {
				last->next = new ReadVariant(referenceIndex,
						string(1, read->seq[readIndex]),
						variantType::SUBSTITUTION, location);
				// cout << "SUBSTITUTION: ";
				// cout << last->next;
				// cout << '\n';
				last = last->next;
			}
			++readIndex;
			++referenceIndex;
			break;
		case cigarState::I:
			insertionString = "";
			while (read->nextCigar() != 1) //Read all but the last one, next Cigar will be new
			{
				insertionString.push_back(read->seq[readIndex]);
				++readIndex;
			}
			insertionString.push_back(read->seq[readIndex]);
			last->next = new ReadVariant(referenceIndex, insertionString,
					variantType::INSETION, location);
			// cout << "INSERTION: ";
			// cout << last->next;
			// cout << '\n';
			last = last->next;
			++readIndex;
			break;
		case cigarState::D:
			last->next = new ReadVariant(referenceIndex, " ",
					variantType::DELETION, location);
			// cout << "DELETION: ";
			// cout << last->next;
			// cout << '\n';
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
	last = first->next;
	cout << "delete 91\n";
	cout << first->toString();
	delete first;
	return last;
}

unsigned long Core::Variant2int(ReadVariant *variant) {
	unsigned long ret = 0;
	if (variant->type == variantType::INSETION) {
		ret += 1;
	}
	ret<<1;
	/*
	 for (size_t i = 0; i < min(variant->bases.length(), maxInsLength); i++)
	 {
	 switch (variant->bases[i])
	 {
	 case 'A':
	 ret+=0;
	 break;
	 case 'G':
	 ret+=1;
	 break;
	 case 'T':
	 ret+=2;
	 break;
	 case 'C':
	 ret+=3;
	 break;
	 default:
	 break;
	 }
	 ret<<2;
	 } // let's suppose there aren't any insertions that differ after 16 same bases...*/
	ret += hasher(variant->bases);
	ret << (sizeof(ret) * 8 / 2);
	ret += variant->index;
	return ret;
}

void Core::reportReadVariant(Read *first, Read *second, ReadVariant *firstRV,
		ReadVariant *secondRV) {
	//cout << "report120\n";
	char refBase = reference->ref[firstRV->index];
	//cout << "report122\n";
	unsigned long firstHash = Variant2int(firstRV);
	unsigned long secondHash = Variant2int(secondRV);
	//cout << "report125\n";
	if (firstHash == secondHash) {
		reference->reportVariant(firstHash, false, false, true, firstRV,
				refBase);
		cout << "delete 145\n";
		cout << secondRV->toString();
		delete secondRV;
	} else {
		reportFirstReadVariant(first, firstRV);
		reportSecondReadVariant(second, secondRV);
	}

}

void Core::reportFirstReadVariant(Read *first, ReadVariant *firstRV) {
	char refBase = reference->ref[firstRV->index];
	unsigned long firstHash = Variant2int(firstRV);

	reference->reportVariant(firstHash, true, false, false, firstRV, refBase);
}

void Core::reportSecondReadVariant(Read *second, ReadVariant *secondRV) {
	char refBase = reference->ref[secondRV->index];
	unsigned long secondHash = Variant2int(secondRV);
	reference->reportVariant(secondHash, false, true, false, secondRV, refBase);
}

void Core::analyzeReads(Read *first) {
	Read *second = first->pair;
	ReadVariant* firstVariant = analyzeRead(first);
	ReadVariant* secondVariant = analyzeRead(second);
	ReadVariant* oldFirstVariant;
	ReadVariant* oldSecondVariant;
	oldFirstVariant = firstVariant;
	oldSecondVariant = secondVariant;
	while (firstVariant != nullptr && secondVariant != nullptr) {
		if (firstVariant->index == secondVariant->index) {
			firstVariant = firstVariant->next;
			secondVariant = secondVariant->next;
			reportReadVariant(first, second, oldFirstVariant, oldSecondVariant);
			cout << "delete 198\n";
			cout << oldSecondVariant->toString();
			delete oldSecondVariant;
		} else if (firstVariant->index < secondVariant->index) {
			cout << firstVariant;
			cout << "f201\n";
			firstVariant = firstVariant->next;
			reportFirstReadVariant(first, oldFirstVariant);
		} else {
			secondVariant = secondVariant->next;
			reportSecondReadVariant(second, oldSecondVariant);
		}
	}
	while (firstVariant != nullptr) {
		firstVariant = firstVariant->next;
		reportFirstReadVariant(first, oldFirstVariant);
	}
	while (secondVariant != nullptr) {
		secondVariant = secondVariant->next;
		reportSecondReadVariant(second, oldSecondVariant);
	}
	cout << "delete 221";
	cout << first->toString();
	delete first; //This will automatically delete also second
}
;
