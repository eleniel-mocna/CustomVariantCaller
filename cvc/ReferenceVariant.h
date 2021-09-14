#pragma once

#include <string>
#include "ReadVariant.h"

using namespace std;

struct ReferenceVariant
{
    string toString();
    ReferenceVariant(ReadVariant* variant, char referenceBase);
    unsigned int position;
    string bases;
    string position_name;
    char referenceBase;
    unsigned int firstCount;
    unsigned int secondCount;
    unsigned int pairsCount;
};
