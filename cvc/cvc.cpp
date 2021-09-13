#include <string>
#include <fstream>
#include <iostream>
#include "Reader.h"
#include "Reference.h"


using namespace std;

int main()
{
    string line;
    Reader reader("../../../data/correct_sam.sam");
    Read newRead = *(reader.getPairReads());
    
    /*Read pair = *(newRead.pair);
    cout << pair.toString();
    Reference refer("../../../data/TP53_F1.fasta");
    Base b[130] = {};
    refer.getSequence(1,130,b);*/
    cout << newRead.toString();
    //Reference* refer = new Reference("../../../data/TP53_F1.fasta");
    Reference* refer = new Reference("../../../data/reference/ucsc.hg19.fasta");
    cout << (*refer).getLength();    
    return 0;
}