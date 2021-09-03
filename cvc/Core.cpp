#include "Core.h"

Core::Core(Reference* ref)
{
    reference = ref;
}

Core::~Core()
{
}

size_t Core::findOffset(int of, int from)
{
    if (of<from)
    {
        return 0;
    }
    else
    {
        return of-from;
    }
    
}

void Core::analyzeReads(Read* first)
{
    //Check if they overlap (we could load the whole reference otherwise :-/)
    Read* second = first->pair;
    Base* refSeq;
    size_t refLength = getReferenceLength(first, second, refSeq);
    size_t firstOffset = findOffset(first->tlen, second->tlen);
    size_t secondOffset = findOffset(second->tlen, first->tlen);
    (*first).configure(firstOffset);
    (*second).configure(secondOffset);




}

size_t Core::getReferenceLength(Read* first, Read* second, Base* ret)
{
    int begginingPos = min(first->pos, second->pos);
    int endPos = max(first->pos + first->tlen, second->pos + second->tlen);
    size_t refLength = endPos - begginingPos;
    
    (*reference).getSequence(begginingPos, refLength, ret);
    return refLength;
}