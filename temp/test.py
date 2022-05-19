# Old   CHROM	POS	REF	ALT	ADF 	        ADR 	        ADP	DP	QDP
# New:  CHROM	POS	REF	ALT	ADFP	ADFU	ADRP	ADRU	ADP	DP	QDP
new = []
with open("temp/newestSorted", "r") as file:
    for line in file:
        new.append(line.split("\t"))

old = []
with open("temp/oldSorted", "r") as file:
    for line in file:
        old.append(line.split("\t"))
print(len(new))
print(len(new[0]))
print(len(old))
print(len(old[0]))

assert(len(new)==len(old))
variant_differences = 0
forward_differences = 0
reverse_differences = 0
summary_differences = 0
for i in range(len(new)):
    new_line = new[i]
    old_line = old[i]    
    for j in (0,1,2,3):
        if new_line[j]!=old_line[j]:
            print(old_line)
            print(new_line)
            variant_differences+=1
    if int(old_line[4]) != int(new_line[4]) + int(new_line[5]):
        print(old_line)
        print(new_line)
        forward_differences+=1
    if int(old_line[5]) != int(new_line[6]) + int(new_line[7]):
        print(old_line)
        print(new_line)
        reverse_differences+=1
    for j in (6,7,8):
        if new_line[j+2]!=old_line[j]:
            print(old_line)
            print(new_line)
            summary_differences+=1

print(f"Variant differences: {variant_differences}")
print(f"Forward differences: {forward_differences}")
print(f"Reverse differences: {reverse_differences}")
print(f"Summary differences: {summary_differences}")

    