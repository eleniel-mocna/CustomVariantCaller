#include "ReadVariant.h"
#include <string>


ReadVariant::ReadVariant()
{
    index=0;
    bases="";
}

ReadVariant::ReadVariant(int index, string bases)
{
    this->index = index;
    this->bases = bases;
}

ReadVariant::~ReadVariant()
{
    delete next;
}
DummyReadVariant::DummyReadVariant()
{}

DummyReadVariant::~DummyReadVariant()
{}