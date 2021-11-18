#!/bin/bash
set -euo pipefail # This is here so we don't fail on "folder exists error"
set -- "${POSITIONAL[@]}" # restore positional parameters

log () 
{
    log_string="[CVC $(date +%H:%M.%S)] $1"
    echo "$log_string">>"$log_file"
    echo "$log_string">&2
}

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
            echo 'USAGE: ./VariantCaller.sh [options] reads1.fastq reads2.fastq.'
            echo 
            echo "Options:"
            echo "  -h,--help			  display this information"
            echo "  -f,--fasta-ref FILE   faidx indexed reference sequence file [\"/reference/ucsc.hg19.fasta\"]"
            echo "  -q,--mapq             skip alignments with mapQ smaller than INT [0]"
            echo "  -Q,--baseq            skip bases with baseQ/BAQ smaller than INT [13]"
            echo "     --threads          number of threads to use. (Disabled)"
            echo "     --no-vcf           the output should not contain a vcf file."
            echo "     --no-tsv           the output should not contain a tsv file."
            echo "  -o,--output-dir       directory name to store output."
            echo "  -b,--from-bam         given only 1 positional argumet, call variants on a bam."
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
        vcf_file=1
        shift
        ;;

    --no-tsv)
        tsv_file=1
        shift
        ;;
    -o|--output-dir)
        name="$2"
        shift
        shift
        ;;
    -b|--from-bam)
        from_bam=1
        shift
        shift
        ;;

    *)   
      POSITIONAL+=("$1")
      shift 
      ;;
  esac
done

mkdir "$name"

bam_file="$name/$name.bam"
log_file="$name/$name.log"


if [ -z ${from_bam+x} ]; then
    reads1=${POSITIONAL[-2]}
    reads2=${POSITIONAL[-1]}
    original_bam="-"
else
    original_bam="${POSITIONAL[-1]}"
    reads1="-"
    reads2="-"
    cp "$original_bam" "$bam_file"
    log "Copied ${POSITIONAL[-1]} to $bam_file"
fi

if [ -z ${vcf_file+x} ]; then
    to_vcf=""
else
    to_vcf="--vcf-file $name/$name.vcf"
fi

if [ -z ${tsv_file+x} ]; then
    to_tsv=""    
else
    to_tsv="--tsv-file $name/$name.tsv"
fi



tmp_file=$(mktemp /tmp/VariantCaller_XXXXXX.bam)


log "--------------------"
log "CVC called into folder $name"
log "reads1:    $reads1"
log "reads2:    $reads2"
log "orig_bam:  $original_bam"
log "reference: $reference" 
log "mapQ:      $mapq" 
log "baseQ:     $baseq"
log "to_vcf:    $to_vcf"
log "to_tsv:    $to_tsv"
log "--------------------"

if [ -z ${from_bam+x} ]; then
    log "Running BWA mem..."
    bwa mem -t 24 "$reference" "$reads1" "$reads2" | samtools view -h | samtools sort -n | samtools view -h -b > "$tmp_file"
    log "BWA mem completed successfully!"
else
    log "Skipping BWA mem as bam was given."
    samtools view -h "$bam_file" | samtools sort -n | samtools view -h -b > "$tmp_file"
fi

log "Calling variants..."
log "samtools view -h $tmp_file | /cvc/VariantCaller.out --reference $reference --mapq $mapq --qual $baseq --threads $threads  $to_vcf $to_tsv - > $name/out.tsv"
samtools view -h $tmp_file | /cvc/VariantCaller.out --reference $reference --mapq $mapq --qual $baseq --threads $threads  $to_vcf $to_tsv - >$name/out.tsv
log "Variants called successfully!"

log "Creating bam index file..."
samtools view -h "$tmp_file" | samtools sort | samtools view -h -b > "$bam_file"
samtools index "$bam_file"
log "Bam index file created successfully!"

log "Removing tempfiles..."
rm "$tmp_file"
log "Tempfiles removed!"

log "CVC ended successfully!"