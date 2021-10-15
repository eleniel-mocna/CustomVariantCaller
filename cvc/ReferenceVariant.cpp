#include "ReferenceVariant.h"
#include "ReadVariant.h"
#include <iostream>

using namespace std;

/**
 * @brief Construct a new Variant object
 * 
 * @param variant What variant should this object represent.
 * @param refBase What is the reference base for this position.
 */
ReferenceVariant::ReferenceVariant(ReadVariant *variant, string refBases) {
	position = variant->index;
	bases.reserve(variant->bases.length());
	for(size_t i = 0; i < variant->bases.size(); ++i)
		if(variant->bases[i] != '\0') bases += variant->bases[i];
	position_name = variant->location;
	referenceBases = refBases;
	firstCount = 0;
	secondCount = 0;
	pairsCount = 0;
	DP=0;
}

/**
 * @brief Set the total depth. Needs to be set before the output is generated.
 * 
 * This shall be called after the whole analysis is finished and before the output.
 * Should be done by Reference.
 * 
 * @param dp 
 */
void ReferenceVariant::addDP(unsigned int dp)
{
	DP=dp;
}
/**
 * @brief Set the quality depth. Needs to be set before the output is generated.
 * 
 * This shall be called after the whole analysis is finished and before the output.
 * Should be done by Reference.
 * 
 * @param qdp 
 */
void ReferenceVariant::addQDP(unsigned int qdp)
{
	QDP=qdp;
}

/**
 * @brief return one line of a vcf file describing this variant.
 * 
 * @note ReferenceVariant::addDP should be called before this, or DP=0.
 * 
 * @return string 
 */
string ReferenceVariant::toString() {
	string ret = "";
	ret += position_name;
	ret += '\t';
	ret += '.'; //ID
	ret += '\t';
	ret += referenceBases;
	ret += '\t';
	ret += bases;
	ret += '\t';
	ret += '.'; //Quality TODO
	ret += '\t';
	ret += '.'; //Filter TODO
	ret += '\t';
	ret += "ADF=" + to_string(firstCount);
	ret += ";ADR=" + to_string(secondCount);
	ret += ";ADP=" + to_string(pairsCount);
	ret += ";DP=" + to_string(DP);
	ret += ";QDP=" + to_string(QDP);
	ret += '\t';
	ret += '.'; // FORMAT
	ret += '\n';
	return ret;
}

string ReferenceVariant::toTSV()
{
	if (bases=="" || bases == "\0")
	{
		cerr << toString();
		cerr << "/\\ THIS IS A PROBLEM!!!\n";
	}
	
	string ret = "";
	ret += position_name;
	ret += '\t';
	ret += referenceBases;
	ret += '\t';
	ret += bases;
	ret += '\t';
	ret += to_string(firstCount) + '\t';
	ret += to_string(secondCount) + '\t';
	ret += to_string(pairsCount) + '\t';
	ret += to_string(DP) + '\t';
	ret += to_string(QDP) + '\t';
	ret += '\n';
	return ret;
}
