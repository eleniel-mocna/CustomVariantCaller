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
public:
    Writer();
    ~Writer();
    void outputVCF(Reference *reference);
    void outputTSV(Reference *reference);
};