
# Custom Variant Caller

This is a variant caller made for analysis of paired-end sequences with special focus on pair / single read support for variants.
It takes a [SAM file](https://en.wikipedia.org/wiki/SAM_(file_format)) sorted by *QNAME* as input and outputs a [VCF file](https://en.wikipedia.org/wiki/Variant_Call_Format#cite_ref-2).
To sort your SAM file use `samtools sort -n [file]`.

## Usage

### Requirements

This script is build for Linux and should run on every reasonable distribution, successfully tested on Ubuntu
To run this script you just need `g++` compiler supporting c++11.
To generate the documentation use [Doxygen 1.9.1](https://www.doxygen.nl/index.html).

### Compilation

To compile the program just run **`make`** in root of this repository.
This should compile the program into **`VariantCaller`** binary.

### Running the script

```man
Usage: VariantCaller [options] SAM_file...
Options:
- -h --help Display this information\n
- -\-reference Path to the reference fasta file (default \"reference/ucsc.hg19.fasta\")
Output:
- vcf file-like string to stdout.
```

## Used algorithms

### Smart comparing

The main algorithm used in this project is the comparison of SAM reads to the reference. There were two main considerations for this algorithm.
Firstly, every read has got only CIGAR string information to use to match the alignment found by the aligner. This contains everything we need but is a little bit complicated to extract.
Secondly, we need to use information from both reads from the found pair to generate variants.

#### Reading the CIGAR string

This algorithm does not read through the bases of a given read, but instead reads through the CIGAR string and gives bases based on these read states.
Along with the CIGAR string we have two indices, readIndex and referenceIndex, which we have to move according to the CIGAR state. When we find a difference in the read and the reference, we have to report said difference as a variant as depicted in the following table:
| CIGAR Code  |                      Description                      | Consumes read  | Consumes reference  | Report variant |
|:-----------:|:-----------------------------------------------------:|:--------------:|:-------------------:|:--------------:|
|      M      | alignment match (can be a sequence match or mismatch) |       yes      |         yes         | (Substitution) |
|      I      |               insertion to the reference              |       yes      |         no          |    Insertion   |
|      D      |              deletion from the reference              |       no       |         yes         |    Deletion    |
|      N      |           skipped region from the reference           |       no       |         yes         |       no       |
|      S      |    soft clipping (clipped sequences present in SEQ)   |       yes      |         no          |       no       |
|      H      |  hard clipping (clipped sequences NOT present in SEQ) |       no       |         no          |       no       |
|      P      |    padding (silent deletion from padded reference)    |       no       |         no          |       no       |
|      =      |                     sequence match                    |       yes      |         yes         |       no       |
|      X      |                   sequence mismatch                   |       yes      |         yes         |  Substitution  |
But because we have two reads in a pair, we do this comparison for each one of them and then just compare variants for each of them.

#### Different algorithms

I have settled on this algorithm for two reasons:

- It is very simple and already used in other variant callers (which eliminates most pitholes)
- It is very expandable, because we can save everything into a concise structure which can then evaluated

On the other hand doing this comparison on both reads at the same time leads to a very complicated code, which is very hard to expand on.

### Storing the variants

After having generated the variants we have to store them in some structure. For this I have first used an array of custom objects for amplicon sequencing, but later switched to a c++ standard hashmap, which saves `O([length of reference] - [# of variants])` memory, which is very close to the length of the reference.
Using this hashmap we can create new variants and increment numbers on the already created ones in average of O(1) time.

## Code segmentation

### Main considerations

As the main consideration of this code is to be easily expandable, I wanted to have one main class, where all the computation (comparison) happens. And then have I/O classes that allow this 'Core' class to be as simple as possible, while giving over all the information that could ever be needed. Also these I/O classes need to be open for parallelization, which I am hoping to implement in the near future.
Also this program is performance critical, because it deals with a lot of data.

Lastly one of the requirements was that this program is written as close to C as possible (because of more knowledge of C over C++ for the main user of this code). This is why some functions of C++ like smart pointers are not used. I have used C++ where it made the code much more readable (such as using classes over structs) and where the usage and performance is very similar to C, but C++ is way more convenient (such as using C++ vectors over C arrays)

### Actual segmentation used

Is described in automatically generated docs for every class.

## Workflow and surprising setbacks

As always there is much more work hidden than shown in this project. But in this one much more then in any other I have written.
This not being a game or a simulation of phenomena I fully understand, the preparations for this project took much more time than I thought. Coming up with the main classes and their connections was easy, but understanding every format and concept in variant calling was much harder.
Because SAM is an old, backwards compatible file format, there are many caveats that don't make any sense to me. For example CIGAR string is the minimal representation of alignment possible. Which means it doesn't have mismatches and matches differentiated.
And the fasta is even worse. Not only can it contain multiple files with only headers (i.e. names of chromosomes) as delimiters, not only there are newlines that are there only for esthetic reasons but there are meanings to every letter of the alphabet and even more.

## Additional work

There is a lot more work to do with this project beginning such as:

- Read input from stdin
- Writing a special hash function for variants
- Adding filtering for quality of reads
- Adding more information to the output variants
- Making I/O classes parallel safe
- Making multiple cores and running analysis in parallel
- Add checks for missformed fasta and SAM files

## Some last words

Does this documentation has the most obnoxious color I managed to find? Maybe... It might just be a tribute to one very old Perl program I used to work with that was made in the 90's and had this exact color. You can change it in the Doxyfile if you want :-).
