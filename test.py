my_name = "my_sorted.tsv"
right_name = "varscan_sorted.tsv"
my = open(my_name, "r")
right = open(right_name, "r")
my_text = my.readline().split('\t')
for line in right:
    text = line.split('\t')
    while int(my_text[1]) < int(text[1]):
        my_text = my.readline().split('\t')
    if int(my_text[1]) == int(text[1]):
        print(text)
        while int(my_text[1]) == int(text[1]):
            print(my_text)
            my_text = my_text = my.readline().split('\t')
        print("-------------")
    else:
        print(text)
        print("###############")
