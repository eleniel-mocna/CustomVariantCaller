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
{
}

Writer::~Writer()
{
}

const string Writer::VCFHeader = "##fileformat=VCFv4.3\n"
			"##INFO=<ID=ADFP,Number=1,Type=Integer,Description=\"Depth of variant-supporting bases on forward strand (reads2plus), pair spans this position\">\n"
			"##INFO=<ID=ADFU,Number=1,Type=Integer,Description=\"Depth of variant-supporting bases on forward strand (reads2plus), pair does NOT span this position\">\n"
			"##INFO=<ID=ADR,Number=1,Type=Integer,Description=\"Depth of variant-supporting bases on forward strand (reads2minus), pair spans this position\">\n"
			"##INFO=<ID=ADR,Number=1,Type=Integer,Description=\"Depth of variant-supporting bases on forward strand (reads2minus), pair does NOT span this position\">\n"
			"##INFO=<ID=ADP,Number=1,Type=Integer,Description=\"Depth of variant-supporting bases on pair reads\">\n"
			"##INFO=<ID=DP,Number=1,Type=Integer,Description=\"Total depth\">\n"
			"#CHROM POS      ID         REF   ALT    QUAL  FILTER   INFO\n";
const string Writer::TSVHeader = "CHROM\tPOS\tREF\tALT\tADFP\tADFU\tADRP\tADRU\tADP\tDP\tQDP\n";

void Writer::outputVCF(Reference *reference)
{
	VCF(reference, &cout);
}
void Writer::outputTSV(Reference *reference)
{
	TSV(reference, &cout);
}
void Writer::fileTSV(Reference *reference, string fileName)
{
	ofstream file;
	file.open(fileName);
	TSV(reference, &file);
	file.close();
}

void Writer::fileVCF(Reference *reference, string fileName)
{
	ofstream file;
	file.open(fileName);
	VCF(reference, &file);
	file.close();
}

void Writer::VCF(Reference *reference, ostream *file)
{
	*file << VCFHeader;
	for (auto varIter = reference->variants->begin(); varIter != reference->variants->end();
		 varIter++)
	{
		ReferenceVariant *var = varIter->second;
		var->addDP(reference->getTotalDepth(var->position));
		var->addQDP(reference->getQTotalDepth(var->position));
		*file << var->toString();
	}
}

void Writer::TSV(Reference *reference, ostream *file)
{
	*file << TSVHeader;
	for (auto varIter = reference->variants->begin(); varIter != reference->variants->end();
		 varIter++)
	{
		ReferenceVariant *var = varIter->second;
		var->addDP(reference->getTotalDepth(var->position));
		var->addQDP(reference->getQTotalDepth(var->position));
		*file << var->toTSV();
	}
}

