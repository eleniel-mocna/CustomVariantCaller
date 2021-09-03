#pragma once

#include <string>
#include <fstream>

using namespace std;
enum class Base {G, T, A, C, EMPTY};

string Base2String(Base*, size_t);
char Base2Char(Base);

class Reference
{
public:
    Reference(string);
    ~Reference();
    void getSequence(int, size_t, Base*);
    int getLength();

private:
    Base* ref;
    int length;
    void setLength(ifstream&);
    void createArray(ifstream&);
    char bases[8] = {'A', 'a', 'G', 'g', 'T', 't', 'C', 'c'};
    void skipLine(ifstream&);

};