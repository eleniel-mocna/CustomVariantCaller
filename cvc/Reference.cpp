#include <iostream>
#include <fstream>
#include <cassert>
#include "Reference.h"
#include "Read.h"
#include <stdexcept>

using namespace std;

char Base2Char(Base b)
{
    switch (b)
    {
    case Base::A:
        return 'A';
        break;
    case Base::G:
        return 'G';
        break;
    case Base::T:
        return 'T';
        break;
    case Base::C:
        return 'C';
        break;
    default:
        return '#';
        break;
    }
}

string Base2String(Base* b, size_t length)
{
    string ret = "";
    for (size_t i = 0; i < length; i++)
    {
        ret.push_back(Base2Char(b[i]));
    }
    return ret;

}

void Reference::setLength(ifstream& file)
{    
    length=0;
    string line;
    while (getline(file, line))
    {
        if (line[0]=='@' || line[0]==';');//This line is a header or comment
        else
        {
            for (char character : line)
            {
                for (char base : bases)
                {
                    if (character==base)
                    {
                        length+=1;
                    }                    
                }
                
            }    
        }   
    }
}

void Reference::skipLine(ifstream& file)
{
    while (file.get() != '\n');
}

void Reference::createArray(ifstream& file)
{
    file.clear();
    file.seekg(0);
    ref = new Base[length];
    char last;   
    int i = 0;
    while (file.get(last))
    {
        switch (last)
        {
        case 'a':
        case 'A':
            ref[i] = Base::A;
            break;
        case 'g':
        case 'G':
            ref[i] = Base::G;
            break;
        case 't':
        case 'T':
            ref[i] = Base::T;
            break;
        case 'c':
        case 'C':
            ref[i] = Base::C;
            break;
        default:
            --i; //This wasn't a base, go back in counting.
            break;
        }
        ++i;
        
    }
    assert(length==i && "Input file wasn't read correctly!");
    
    


}

Reference::Reference(string path)
{ 
    ifstream file(path);
    setLength(file);
    createArray(file);
}

Reference::~Reference()
{
    delete [] ref;
}

void Reference::getSequence(int pos, size_t lengthOfSeq, Base* ret)
{
    if (pos + lengthOfSeq > length)
    {
        size_t overflowAmount = lengthOfSeq - (length - pos);
        for (size_t i = lengthOfSeq - overflowAmount; i < lengthOfSeq; i++)
        {
            ret[i] = Base::EMPTY;
        }        
        lengthOfSeq -= overflowAmount;    
    }
        
    for (size_t i = 0; i < lengthOfSeq; i++)
    {   
        ret[i] = ref[pos+i];
    }    
}