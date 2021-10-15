/*
 * Writer.h
 *
 *  Created on: Sep 30, 2021
 *      Author: samuel
 */
#pragma once

#include "Reference.h"

class Writer
{
private:
    static const string VCFHeader;
    static const string TSVHeader;
    static void VCF(Reference *reference, ostream *file);
    static void TSV(Reference *reference, ostream *file);
public:
    Writer();
    ~Writer();
    static void outputVCF(Reference *reference);
    static void outputTSV(Reference *reference);
    static void fileTSV(Reference *reference, string fileName);
    static void fileVCF(Reference *reference, string fileName);
};