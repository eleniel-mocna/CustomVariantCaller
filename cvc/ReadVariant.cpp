#include "ReadVariant.h"
#include <string>
#include <iostream>

string variantType2string(variantType type)
{
    switch (type)
    {
    case variantType::DELETION:
        return "deletion";
        break;
    case variantType::INSETION:
        return "insertion";
        break;
    case variantType::SUBSTITUTION:
        return "substitution";
        break;
    default:
        return "unknown";
    }
}

ReadVariant::ReadVariant() //This shall be called only as the head of a linked list
{                          // This ain't no pretty, but, ya'll, it's pretty short!
    index=0;               // If we created a child class, its destructor would call this ones no matter what...
    bases="";
    next=nullptr;
}

ReadVariant::ReadVariant(unsigned int index, string bases, variantType type, string loc)
{
    cout << this;
    cout << ": " + toString();
    this->index = index;
    this->bases = bases;
    this->type = type;
    this->location = loc;
    next=nullptr;
}

string ReadVariant::toString()
{
    return "ReadVariant: " + variantType2string(type) + " @ " + to_string(index) + ", " + bases + ", next: " + to_string(bool(next)) + '\n';
}

ReadVariant::~ReadVariant()
{
}