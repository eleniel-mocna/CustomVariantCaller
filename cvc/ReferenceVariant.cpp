#include "ReferenceVariant.h"
#include "ReadVariant.h"

using namespace std;

ReferenceVariant::ReferenceVariant(ReadVariant* variant, char refBase)
{
    position = variant->index;
    bases = variant->bases;
    position_name = variant->location;
    referenceBase = refBase;
    firstCount = 0;
    secondCount = 0;
    pairsCount = 0;
}

string ReferenceVariant::toString()
{
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
    ret += '\t';
    ret += '.'; // FORMAT
    return ret;
}