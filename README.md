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
