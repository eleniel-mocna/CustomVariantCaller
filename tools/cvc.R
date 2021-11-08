# reads1/2 -> path to fastq files for variant calling
# NAME -> path to folder in which these should be stored
# reference -> path to reference
# mapQ -> minimal reads mapping quality to be considered
# baseQ -> minimal base quality to be considered
# no_vcf -> don't bother outputting the vcf file
# no_tsv -> don't bother outputting the tsv file

cvc <- function(reads1,
                reads2,
                NAME=NULL,
                reference="/reference/ucsc.hg19.fasta",
                mapQ=0,
                baseQ=13,
                no_vcf=NULL,
                no_tsv=NULL)
{
    if(is.null(NAME)) {o = ""}
    else {o = "-o"}

    if(is.null(no_vcf)) {vcf=""}
    else {vcf = "--no-vcf"}

    if(is.null(no_tsv)) {tsv=""}
    else {tsv = "--no-tsv"}
    system(paste("/cvc/tools/pipe.sh",
                 "-f", reference,
                 "-q", mapQ,
                 "-Q", baseQ,
                 o, NAME,
                 vcf,
                 tsv,
                 reads1,
                 reads2            
                  ))
}

cvc_from_bam <- function(bam_file,
                        NAME=NULL,
                        reference="/reference/ucsc.hg19.fasta",
                        mapQ=0,
                        baseQ=13,
                        no_vcf=NULL,
                        no_tsv=NULL)
{
    if(is.null(NAME)) {o = ""}
    else {o = "-o"}

    if(is.null(no_vcf)) {vcf=""}
    else {vcf = "--no-vcf"}

    if(is.null(no_tsv)) {tsv=""}
    else {tsv = "--no-tsv"}
    system(paste("/cvc/tools/pipe.sh",
                 "-b",
                 "-f", reference,
                 "-q", mapQ,
                 "-Q", baseQ,
                 o, NAME,
                 vcf,
                 tsv,
                 bam_file         
                  ))
}                        

cvc_fancy <- function(table){
    tab <- read.table(table, sep="\t", header = TRUE)
    reads1 <- tab[,"Fastq1"][1]
    reads2 <- tab[,"Fastq2"][1]
    names <- list("Reference", "MapQ", "BaseQ", "Sample_ID")
    operators <- list("-f", "-q", "-Q", "-o")
    values <- list()
    for (i in 1:length(names))
    {
        name <- names[[i]]
        operator <- operators[[i]]
        if(name %in% colnames(tab)) 
        {
            values[[i]] = paste(operator, tab[,name][1])
        } else { values[[i]] = "" }
    }
    system(paste("/cvc/tools/pipe.sh",
                 paste(values, collapse = ", "),
                 reads1, reads2))
}