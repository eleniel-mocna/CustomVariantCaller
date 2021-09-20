/*
 * cvc.cpp
 *
 *  Created on: Sep 20, 2021
 *      Author: samuel
 */

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Core.h"
#include "Reader.h"
#include "Reference.h"

using namespace std;
/**
 * @brief Class for parsing input arguments
 * 
 * Taken from https://stackoverflow.com/users/85381/iain @
 *      https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
 * 
 */
class InputParser{
    public:
        ///
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(string(argv[i]));
        }
        /// @author iain
        const string& getCmdOption(const string &option) const{
            vector<string>::const_iterator itr;
            itr =  find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmdOptionExists(const string &option) const{
            return find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        vector <string> tokens;
};

/**
 * @brief Output variants including # of supporting pairs and single reads for each variant. 
 * 
 * @param refPath Path to the reference.
 * @param samPath Path to the SAMfile.
 */
void callVariants(string refPath, string samPath) {
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
    cerr << "Variants finished, program ended succesfully!\n";
}

int main(int argc, char **argv){
    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h") || input.cmdOptionExists("--help") ||
        argc==1){
        cout << "Usage: VariantCaller [options] SAM_file...\n"
                "Options:\n"
                "    -h --help			Display this information\n"
                "    --reference		Path to the reference fasta file (default \"reference/ucsc.hg19.fasta\")\n";
        return 0;
    }
    string referenceFilename = input.getCmdOption("--reference");
    if (referenceFilename.empty()){
        referenceFilename = "reference/ucsc.hg19.fasta";
    }
    string samFilename = argv[argc - 1];
    
    
    cout << "##fileformat=VCFv4.3\n" // TODO move this into a file and own function
            "##INFO=<ID=FC,Number=1,Type=Integer,Description=\"Number of first reads supporting variant\">\n"
            "##INFO=<ID=SC,Number=1,Type=Integer,Description=\"Number of second reads supporting variant\">\n"
            "##INFO=<ID=PC,Number=1,Type=Integer,Description=\"Number of whole pairs supporting variant\">\n"
            "##INFO=<ID=DP,Number=1,Type=Integer,Description=\"Total depth\">\n"
            "#CHROM POS      ID         REF   ALT    QUAL  FILTER   INFO\n";
    callVariants(referenceFilename, samFilename);
    return 0;
}