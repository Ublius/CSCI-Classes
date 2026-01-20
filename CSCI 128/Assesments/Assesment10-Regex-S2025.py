import re

def dna_to_rna(sequence):
    conversions = {'A': 'U', 'T': 'A', 'C': 'G', 'G': 'C'}
    # Find all DNA in sequence
    bases = re.findall(r'[ATCG]', sequence)
    #Replace DNA with RNA
    return "".join(conversions[b] for b in bases)

def parse_file_into_acids(filename):
    filelist = []
    with open(filename) as myfile:
        for row in myfile:
            filelist.append(row.split())
    return filelist

if __name__ == '__main__':
    codonsFilename = input("CODON FILENAME> ")
    sequenceFilename = input("SEQUENCE FILENAME> ")
    output_file = input("OUTPUT FILE> ")

    acids = parse_file_into_acids(codonsFilename)

    with open(output_file, 'w') as file:
        with open(sequenceFilename, 'r') as myfile:
            while True:
                sequence = myfile.readline().strip()
                output = sequence + " "

                if sequence == "DONE":
                    break

                sequence = dna_to_rna(sequence)

                # Regex: start at AUG, gather codons until stop codon
                match = re.search(r'AUG(?:(?!UAA|UGA|UAG)...)*', sequence)

                if not match:
                    file.write(output + "\n")
                    continue

                full_codon_region = match.group(0)
                # print("TEST: ", match.group(0))
                # Split into codons
                codons = re.findall(r'...', full_codon_region)
                # print("TEST: ", codons)

                for codon in codons:
                    for acid in acids:
                        if codon == acid[0]:
                            output += acid[2]

                file.write(output + "\n")
