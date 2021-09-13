#pragma once

#include "Read.h"
#include "Reader.h"
#include "Reference.h"
#include "ReadVariant.h"
#include <string>
using namespace std;

class Core
{
private:
    ReadVariant* analyzeRead(Read* first);
    Reference* reference;
    void reportReadVariant(Read* first, Read* second, ReadVariant* firstRV, ReadVariant* secondRV);
    void reportFirstReadVariant(Read* first, ReadVariant* firstRV);
    void reportSecondReadVariant(Read* first, ReadVariant* secondRV);
    unsigned long Variant2int(ReadVariant* variant);
public:
    Core(Reference*);
    ~Core();
    void analyzeReads(Read*);
};

