import re

def dna_to_rna(sequence):
    conversions = {'A': 'U', 'T': 'A', 'C': 'G', 'G': 'C'}
    # Find all the dna
    bases = re.findall(r'[ATCG]', sequence)
    # print("TEST: ", bases)
    # Replace the dna with the rna
    return "".join(conversions[b] for b in bases)

def parse_file_into_acids(filename):
    filelist = []
    with open(filename) as myfile:
        for row in myfile:
            newrow = row.split()
            filelist.append(newrow)
    return filelist

if __name__ == '__main__':
    codonsFilename = input("CODON FILENAME> ")
    sequenceFilename = input("SEQUENCE FILENAME> ")
    output_file = input("OUTPUT FILE> ")

    acids = parse_file_into_acids(codonsFilename)

    start_codon = "AUG"
    stop_codons = ["UAA", "UGA", "UAG"]

    with open(output_file, 'w') as file:
        with open(sequenceFilename, 'r') as myfile:
            while True:
                sequence = myfile.readline().strip()
                output = sequence + " "

                if sequence == "DONE":
                    break

                sequence = dna_to_rna(sequence)
                
                # Find start codon and all codons via regex
                # Find AUG and everything after in groups of 3
                match = re.search(r'AUG(?:...)*', sequence)

                if not match:
                    # No valid start codon found
                    file.write(output + "\n")
                    continue

                full_codon_region = match.group(0)
                # print("TEST: ", match)
                # Split into codons of length 3
                codons = re.findall(r'...', full_codon_region)
                # print("TEST: ", codons)

                final_codons = []
                for c in codons:
                    if c in stop_codons:
                        break
                    final_codons.append(c)

                for codon in final_codons:
                    for acid in acids:
                        if codon == acid[0]:
                            output += acid[2]

                file.write(output + "\n")
