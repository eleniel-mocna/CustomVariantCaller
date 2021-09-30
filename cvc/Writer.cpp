/*
 * Writer.cpp
 *
 *  Created on: Sep 30, 2021
 *      Author: samuel
 */
#include "Writer.h"
#include <iostream>
#include "ReferenceVariant.h"
#include "Reference.h"

using namespace std;


Writer::Writer()
{   }

Writer::~Writer()
{   }

void Writer::outputVCF(Reference *reference)
{
    cout << "##fileformat=VCFv4.3\n"
        "##INFO=<ID=FC,Number=1,Type=Integer,Description=\"Number of first reads supporting variant\">\n"
        "##INFO=<ID=SC,Number=1,Type=Integer,Description=\"Number of second reads supporting variant\">\n"
        "##INFO=<ID=PC,Number=1,Type=Integer,Description=\"Number of whole pairs supporting variant\">\n"
        "##INFO=<ID=DP,Number=1,Type=Integer,Description=\"Total depth\">\n"
        "#CHROM POS      ID         REF   ALT    QUAL  FILTER   INFO\n";
	for (auto varIter = reference->variants->begin(); varIter != reference->variants->end();
			varIter++) {
		ReferenceVariant *var = varIter->second;
		var->addDP(reference->getTotalDepth(var->position));
		var->addQDP(reference->getQTotalDepth(var->position));
		cout << var->toString();
	}
}
void Writer::outputTSV(Reference *reference)
{
    for (auto varIter = reference->variants->begin(); varIter != reference->variants->end();
			varIter++) {
		ReferenceVariant *var = varIter->second;
		var->addDP(reference->getTotalDepth(var->position));
		var->addQDP(reference->getQTotalDepth(var->position));
		cout << var->toTSV();
	}
}

