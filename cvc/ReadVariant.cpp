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
    default:
        return "substitution";
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
    if (index==0) // This is an empty variant - created only in the head of a linked list.
    {    }
    else
    {
        delete next;
    }
}