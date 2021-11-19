# reads1/2 -> path to the fastq files for variant calling
# NAME -> path to the folder in which the results should be stored ["CVC<date>-<time>"]
# reference -> path to reference ["/reference/ucsc.hg19.fasta"]
# mapQ -> minimal reads mapping quality to be considered [0]
# baseQ -> minimal base quality to be considered [13]
# no_vcf -> don't bother outputting the vcf file [False]
# no_tsv -> don't bother outputting the tsv file [False]

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
    for (i in 1:dim(tab)[1])
        {
        reads1 <- tab[,"Fastq1"][i]
        reads2 <- tab[,"Fastq2"][i]
        names <- list("Reference", "MapQ", "BaseQ", "Sample_ID")
        operators <- list("-f", "-q", "-Q", "-o")
        values <- list()
        for (j in 1:length(names))
        {
            name <- names[[j]]
            operator <- operators[[j]]
            if(name %in% colnames(tab)) 
            {
                values[[j]] = paste(operator, tab[,name][i])
            } else { values[[j]] = "" }
        }
        system(paste("/cvc/tools/pipe.sh",
                    paste(values, collapse = ", "),
                    reads1, reads2))
        }
}