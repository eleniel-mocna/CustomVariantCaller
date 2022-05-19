#!/bin/bash
print_help()
{            
    echo
    echo 'This script creates a bam_filter docker with given arguments.'
    echo 'USAGE: ./create_docker.sh <port> <name> <mount_directory> <reference_directory> <password> '
    echo
    echo '  port:            Port for rstudio to be exposed on.' 
    echo '  name:            Name of the created docker.'
    echo '  mount_directory: Directory to be mounted from this filesystem.'
    echo '  reference_directory: Direcory in which ucsc.hg19.fasta is located.'
    echo '  password:        Password for rstudio.'
    echo
    exit 1
}

#https://stackoverflow.com/a/14203146
if [[ $# -lt 4 ]]; then
    print_help
fi

port=$1
name=$2
mount=$3
ref_mount=$4
password=$5

while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
    -h|--help)
            print_help
        ;;    
    *)   
      POSITIONAL+=("$1")
      shift 
      ;;
  esac
done



docker run -e PASSWORD="$password" -p "$port":8787 --cpus=20 -m=200g -d --name "$name" -v "$mount":/home/rstudio/data -v "$ref_mount":/reference cvc_eleniel