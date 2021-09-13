#pragma once

#include <string>

using namespace std;


struct ReadVariant
{
    ReadVariant();
    ReadVariant(int, string);
    ~ReadVariant();
    int index;
    string bases;
    ReadVariant* next;
};

struct DummyReadVariant : public ReadVariant
{
    DummyReadVariant();
    ~DummyReadVariant();
};