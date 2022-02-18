# Custom Variant Caller

## Description

This is a variant caller made for analysis of paired-end sequences with special focus on pair / single read support for variants.

## Installation

### Docker installation (recomended)

The easiest way to run this filter is using an rstudio/rocker docker interface.

- Firstly copy a ucsc.hg19 reference into `reference` directory in the root directory.
- Then build the docker using: `docker build -t cvc_eleniel .`
- After it is built create a new container using _create\_docker.sh_ script: `./create_docker.sh <port> <name> <mount_directory> <password>`
Now you can connect to this docker via rocker/rstudio interface on your chosen port (Most often <http://localhost>:\<port>). In case of any difficulties during the setup see [rocker/rstudio website](https://hub.docker.com/r/rocker/rstudio) for more information.

### Installation into an existing unix enviroment

This is also an option, but it needs a lot more work in modifying the scripts in the `tools` directory. All the scripts need to be modified by changing the paths to the existing scripts, which are added to the root directory in the docker installation.

## Usage

This utility provides two interfaces, 1) functional interface for R, 2) lower level bash script.

### R functions

There are provided 3 functions in R: `cvc(...)` `cvc_from_bam(...)` `cvc_fancy(...)`

### cvc(...)

This function takes two fastQ files, calls a bwa mem algorithm on them and then calls variants on the resulting BAM file.

- `reads1` -> path  to  the first fastq file for  variant  calling
- `reads2` -> path  to the second fastq  file for variant calling
- `NAME` -> path  to the  folder  in  which  the results  should  be  stored ["CVC\<date>-\<time>"]
- `reference` -> path  to  reference ["/reference/ucsc.hg19.fasta"]
- `mapQ` -> minimal  reads  mapping  quality  to  be  considered [0]
- `baseQ` -> minimal  base  quality  to  be  considered [13]
- `no_vcf` -> don't bother outputting the vcf file [False]
- `no_tsv` -> don't  bother  outputting  the  tsv  file [False]

### cvc_from_bam(...)

This function has got the same functionality as above mentioned `cvc(...)`, only it takes a bam file on the input instead of two fastQ files.

- `bam_file` -> path to the bam file on which the variants should be called.
- `NAME` -> path  to the  folder  in  which  the results  should  be  stored ["CVC\<date>-\<time>"]
- `reference` -> path  to  reference ["/reference/ucsc.hg19.fasta"]
- `mapQ` -> minimal  reads  mapping  quality  to  be  considered [0]
- `baseQ` -> minimal  base  quality  to  be  considered [13]
- `no_vcf` -> don't bother outputting the vcf file [False]
- `no_tsv` -> don't  bother  outputting  the  tsv  file [False]

### cvc_fancy(...)

This function changes the method of input arguments. It takes as the input a tsv table with one header row and then rows for individual variant callings.
The required columns are: `"Fastq1"`, `"Fastq2"`, which define the fastQ files that should be analyzed.
Optional columns are:

- `"Sample_ID"`-> equivalent of `NAME` in the aforementioned function `cvc`
- `"MapQ"`-> equivalent of `mapQ` in the aforementioned function`cvc`
- `"BaseQ"`-> equivalent of `baseQ` in the aforementioned function `cvc`
- `"Reference"`-> equivalent of `reference` in the aforementioned function `cvc`

## Detailed functionality description

In the core of this utility is a variant caller, which works takes following steps:

- Read given sam file line by line,
- Filter out all reads that:
  - have empty CIGAR string or,
  - have flag 2048 (supplementary alignment) set;
- If two following lines have the same QNAME handle them as pairs, else handle them as reads without a pair,
- Find variants in every pair/read and increment counters for every position,
- If mapQ of a read is lower than filter level, for this read, increment only counters that don't pass quality checks,
- If baseQ for a base in a read is lower than filter level, for this position in this read, increment only counters that don't pass quality checks,
- Print out all found variants,
- Hopefully don't crash catastrofically in the process :-).

## Output abbreviations meanings

- ADFP (**A**lternate alleles **D**epth on **F**orward strand, **P**air spans this position):
 Depth of variant-supporting bases on forward strand (reads2plus), where the pair read spans this position and doesn't report it.
- ADFU (**A**lternate alleles **D**epth on **F**orward strand, pair does **N**ot span this position):
 Depth of variant-supporting bases on forward strand (reads2plus), where the pair read does not span this position.
- ADRP (**A**lternate alleles **D**epth on **R**everse strand, **P**air spans this position):
 Depth of variant-supporting bases on reverse strand (reads2minus), where the pair read spans this position and doesn't report it.
- ADRU (**A**lternate alleles **D**epth on **R**everse strand, pair does **N**ot span this position):
 Depth of variant-supporting bases on reverse strand (reads2minus), where the pair read does not span this position.
- ADP: Depth of variant-supporting bases on both reads in the pair.
- DP: Total depth
