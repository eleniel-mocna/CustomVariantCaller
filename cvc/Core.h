#pragma once

#include "Read.h"
#include "Reader.h"
#include "Reference.h"
#include <string>
using namespace std;

class Core
{
private:
    size_t getReferenceLength(Read*, Read*, Base*);
    size_t findOffset(int, int);
    Reference* reference;
public:
    Core(Reference*);
    ~Core();
    void analyzeReads(Read*);
};

