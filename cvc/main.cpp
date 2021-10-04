/*
 * cvc.cpp
 *
 *  Created on: Sep 20, 2021
 *      Author: samuel
 */
#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Core.h"
#include "Reader.h"
#include "Reference.h"
#include <thread>
#include "Writer.h"

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
void callVariants(string refPath, string samPath, size_t minMapQ, size_t minQual, size_t nThreads, bool vcf) {
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	cerr << "CVC started!\n";
	ofstream writeFile;
	Reader* reader = new Reader(samPath);
	Reference *refer = new Reference(
			refPath, minMapQ, minQual);
    vector<thread*> *threads = new vector<thread*>(nThreads);
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cerr << "Reference built after " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
	cerr << "Building Core\n";
    Core core(refer, reader);
	cerr << "Core built, geting variants\n";
    // for (size_t i = 0; i < nThreads; i++)
    // {
    //     (*threads)[i] = new thread(core);
    // }
    end = chrono::steady_clock::now();
    cerr << "Cores started after " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    cerr << "Threads started\n";
    core();
    // for (size_t i = 0; i < nThreads; i++)
    // {
    //     cerr << "waiting for thread " + to_string(i) + "\n";
    //     (*threads)[i]->join();
    // }
    
    end = chrono::steady_clock::now();
    cerr << "Threads finished after " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    cerr << "All threads have finished, doing output!\n";
	Writer().outputTSV(refer);
    cerr << "Variants finished, program ended succesfully!\n";
    end = chrono::steady_clock::now();
    cerr << "Program done after " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
}

int main(int argc, char **argv){
    InputParser input(argc, argv);
    if(input.cmdOptionExists("-h") || input.cmdOptionExists("--help") ||
        argc==1){
        cout << "Usage: VariantCaller [options] SAM_file...\n"
                "Options:\n"
                "    -h --help			Display this information\n"
                "    --reference		Path to the reference fasta file (default \"/reference/ucsc.hg19.fasta\")\n"
                "    --mapq             Minimum mapping quality of reads to be considered.\n"
                "    --threads          Number of threads to run with. (Disabled)\n"
                "    --vcf              The output should be formated as vcf, false for tsv [true].\n";
        return 0;
    }
    string referenceFilename = input.getCmdOption("--reference");
    if (referenceFilename.empty()){
        referenceFilename = "/reference/ucsc.hg19.fasta";
    }
    size_t minMapQ;
    try
    {
        minMapQ = stoi(input.getCmdOption("--mapq"));
    }
    catch(const exception& e)
    {
        minMapQ = 0;
    }
    size_t minQual;
    try
    {
        minQual = stoi(input.getCmdOption("--qual"));
    }
    catch(const exception& e)
    {
        minQual = 0;
    }
    string samFilename = argv[argc - 1];

    bool vcf;
    try
    {
        if (toupper(input.getCmdOption("--vcf")[0])=='F'
        || input.getCmdOption("--vcf")[0]=='0')
        {
            vcf = false;
        }
        else if (toupper(input.getCmdOption("--vcf")[0])=='T'
        || input.getCmdOption("--vcf")[0]=='1'
        || referenceFilename.empty())
        {
            vcf = true;
        }
        else
        {
            cerr << "VCF not understood, exporting as vcf\n";
            vcf = true;
        }                
    }
    catch(const std::exception& e)
    {
        vcf = true;
    }
    

    
    callVariants(referenceFilename, samFilename, minMapQ, minQual, 20, vcf);
    return 0;
}