#include "Core.h"
#include <string>

Core::Core(Reference* ref)
{
    reference = ref;
}

Core::~Core()
{
}

ReadVariant* Core::analyzeRead(Read* read)
{
    ReadVariant* first = new DummyReadVariant(); // This makes the code cleaner later in this function :-)
    ReadVariant* last = first;
    int referenceIndex = read->pos;
    char* thisReference;
    reference->getSequence(referenceIndex, read->tlen, thisReference);
    
    string insertionString;
    size_t readIndex = 0;
    while (read->nextCigar())
    {
        switch (read->cigarType)
        {
            case cigarState::M: case cigarState::EQ: case cigarState::X:
                if (thisReference[referenceIndex] == read->seq[readIndex])
                {}
                else 
                {
                    last->next = new ReadVariant(referenceIndex, string(1, read->seq[readIndex]));
                }
                ++readIndex;
                ++referenceIndex;            
                break;
            case cigarState::I:
                insertionString = "";
                while (read->nextCigar()!=1) //Read all but the last one, next Cigar will be new
                {
                    insertionString.push_back(read->seq[readIndex]);
                    ++readIndex;
                }
                insertionString.push_back(read->seq[readIndex]);
                ++readIndex;
                break;                
            case cigarState::D:
                last->next = new ReadVariant(referenceIndex, string(1, read->seq[readIndex]));
                ++referenceIndex;
                break;
            case cigarState::N:
                ++referenceIndex;
                break;
            case cigarState::S:
                ++readIndex;
                break;
            case cigarState::H: case cigarState::P:
                break;
            default:
                break;
            }
    }
    last = first->next;
    delete first; // This won't delete whole linked list, because first member is a dummy
    return first;
    
    
}
unsigned long Core::Variant2int(ReadVariant* variant)
{
    unsigned long ret = 0;
    for (size_t i = 0; i < variant->bases.length(); i++)
    {
        switch (variant->bases[i])
        {
        case 'A':
            ret+=0;
            break;
        case 'G':
            ret+=1;
            break;
        case 'T':
            ret+=2;
            break;
        case 'C':
            ret+=3;
            break;
        default:
            break;
        }
        ret<<2;
    } // let's suppose there aren't any insertions longer than 16 bases...
    ret += variant->index;
    ret << (sizeof(ret)*8/2);
    ret += variant->index;
    return ret;    
}

void Core::reportReadVariant(Read* first, Read* second, ReadVariant* firstRV, ReadVariant* secondRV)
{

}

void Core::reportFirstReadVariant(Read* first, ReadVariant* firstRV)
{

}
void Core::reportSecondReadVariant(Read* second, ReadVariant* secondRV)
{

}

void Core::analyzeReads(Read* first)
{
    Read* second = first->pair;
    ReadVariant* firstVariant = analyzeRead(first);
    ReadVariant* secondVariant = analyzeRead(second);
    while (firstVariant != nullptr && secondVariant != nullptr)
    {
        if (firstVariant->index==secondVariant->index)
        {
            reportReadVariant(first, second, firstVariant, secondVariant);
            firstVariant=firstVariant->next;
            secondVariant=secondVariant->next;
        }
        else if (firstVariant->index<secondVariant->index)
        {
            reportFirstReadVariant(first, firstVariant);
            firstVariant=firstVariant->next;
        }
        else
        {
            reportSecondReadVariant(second, secondVariant);
            secondVariant=secondVariant->next;
        }
    }
    while (firstVariant != nullptr)
    {
        reportFirstReadVariant(first, firstVariant);
        firstVariant=firstVariant->next;
    }
    while (secondVariant != nullptr)
    {
        reportSecondReadVariant(second, secondVariant);
        secondVariant=secondVariant->next;
    }
};