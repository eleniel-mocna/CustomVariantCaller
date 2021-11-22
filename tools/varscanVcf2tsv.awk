# This script converts a VCF generated by varscan to a tsv comparable to tsv output of this
# variant caller.
# It doesn't have any sanity checks, it is NOT written for long sustinability
# If it shows that VC works, it might not, if it shows it doesn't work, it still might.
BEGIN{
}

{
    if ($1~"^#") 
    {
        next
    }
    else 
    {
        printf $1 #Chr name
        printf "\t"
        printf $2 #chr pos
        printf "\t"
        printf $4 # ref bases
        printf "\t"
        printf $5 # variant bases
        printf "\t"
        split($10   , INFO, ":")
        printf INFO[13] # forward strand depth of supporting variant
        printf "\t"
        printf INFO[14] # reverse strand depth of supporting variant
        printf "\t"
        printf "." # pairs supporting variants
        printf "\t"
        printf INFO[3] # 
        printf "\t"
        printf INFO[4]
        printf "\n"

    }
}