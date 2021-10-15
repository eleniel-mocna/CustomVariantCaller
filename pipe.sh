#!/bin/bash

reference="/reference/ucsc.hg19.fasta"
mapq=0
baseq=13
threads=0
vcf_file="output.vcf"
tsv_file="output.tsv"
name="CVC$(date +%y%m%d-%H%M)"

#https://stackoverflow.com/a/14203146
POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
    -h|--help)
            echo
            echo 'This script takes fastq files, aligns them and calls variants.'
            echo 'It reports not only forward/reverse strand support but also pair support.'
            echo 
            echo 'USAGE: ./VariantCaller.sh [options] reads1.fastq reads2.fastq output_folder'
            echo 
            echo "Options:"
            echo "  -h,--help			  display this information"
            echo "  -f,--fasta-ref FILE   faidx indexed reference sequence file [\"/reference/ucsc.hg19.fasta\"]"
            echo "  -q,--mapq             skip alignments with mapQ smaller than INT [0]"
            echo "  -Q,--baseq            skip bases with baseQ/BAQ smaller than INT [13]"
            echo "     --threads          number of threads to use. (Disabled)"
            echo "     --no-vcf           the output should not contain a vcf file [false]."
            echo "     --no-tsv           the output should not contain a tsv file [false]."
            echo "  -o,--output_dir       directory name to store output."
        exit 0
        ;;
    -f|--fasta-ref)
        reference="$2"
        shift
        shift
        ;;

    -q|--mapq)
        mapq="$2"
        shift
        shift
        ;;

    -Q|--baseq)
        baseq="$2"
        shift
        shift
        ;;

    --threads)
        threads="$2"
        shift
        shift
        ;;

    --no-vcf)
        vcf_file=""
        shift
        ;;

    --no-tsv)
        tsv_file=""
        shift
        ;;
    -o|--output_dir)
        name="$2"
        shift
        shift
        ;;

    *)   
      POSITIONAL+=("$1")
      shift 
      ;;
  esac
done

set -- "${POSITIONAL[@]}" # restore positional parameters

reads1=${POSITIONAL[-2]}
reads2=${POSITIONAL[-1]}
bam_file="$name/$name.bam"
if [ vcf_file ]; then
    to_vcf='--vcf-file '$name'/'$vcf_file
fi

if [[ tsv_file ]]; then
    to_tsv='--vcf-file '$name'/'$tsv_file
fi

tmpfile=$(mktemp /tmp/VariantCaller_XXXXXX.bam)

mkdir "$name"
echo 1
bwa mem "$reference" "$reads1" "$reads2" | samtools view -h | samtools sort -n | samtools view -h -b > "$bam_file"
echo 25
samtools view -h "$bam_file" | ./VariantCaller.out --reference "$reference" --mapq "$mapq" --baseq "$baseq" --threads "$threads"  "$to_vcf" "$to_tsv" - > /dev/nul
echo 3
samtools view -h "$bam_file" | samtools sort | samtools view -h -b > "$bam_file"
echo 4
samtools index "$bam_file"
echo 5
rm "$tmpfile"