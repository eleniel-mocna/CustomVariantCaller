#include "ReferenceVariant.h"
#include "ReadVariant.h"

using namespace std;

/**
 * @brief Construct a new Variant object
 * 
 * @param variant What variant should this object represent.
 * @param refBase What is the reference base for this position.
 */
ReferenceVariant::ReferenceVariant(ReadVariant *variant, char refBase) {
	position = variant->index;
	bases = variant->bases;
	position_name = variant->location;
	referenceBase = refBase;
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
	ret += referenceBase;
	ret += '\t';
	ret += bases;
	ret += '\t';
	ret += '.'; //Quality TODO
	ret += '\t';
	ret += '.'; //Filter TODO
	ret += '\t';
	ret += "FC=" + to_string(firstCount);
	ret += ";SC=" + to_string(secondCount);
	ret += ";PC=" + to_string(pairsCount);
	ret += ";DP=" + to_string(DP);
	ret += '\t';
	ret += '.'; // FORMAT
	ret += '\n';
	return ret;
}
