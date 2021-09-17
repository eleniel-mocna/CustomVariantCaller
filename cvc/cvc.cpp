/*
 * cvc.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: samuel
 */

#include <string>
#include <fstream>
#include <iostream>
#include "Core.h"
#include "Reader.h"
#include "Reference.h"

using namespace std;
	
/**
 * @brief Output variants including # of supporting pairs and single reads for each variant. 
 * @param argc # of command line arguments given. 
 * @param argv[1] path to reference.
 * @param argv[2] path to the sam file.
 * 
 * @todo Add flags settings etc.
 * @todo Make stdin as the base input option.
 */
int main(int argc, char** argv) {

	string refPath;
	string samPath;
	if(argc==3)
	{
		refPath=argv[1];
		samPath=argv[2];
	}
	cerr << "cvc started!\n";
	ofstream writeFile;
	Reader reader(samPath);
	Reference *refer = new Reference(
			refPath);
	cerr << "Building Core\n";
	Core core(refer);
	cerr << "Core built, geting variants\n";
	Read *newRead;
	newRead = reader.getPairReads();
	while (newRead != nullptr){
		core.analyzeReads(newRead);
		newRead = reader.getPairReads();
	}
	
	cout << refer->outputVariants();
	return 0;
}
