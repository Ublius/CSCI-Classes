import re

def dna_to_rna(sequence):
    conversions = {'A': 'U', 'T': 'A', 'C': 'G', 'G': 'C'}
    return ''.join(conversions[n] for n in sequence)

def parse_file_into_acids(filename):
    with open(filename) as myfile:
        return [line.split() for line in myfile]

if __name__ == '__main__':
    filename = input("FILENAME> ")
    output_file = input("OUTPUT FILE> ")

    acids = parse_file_into_acids(filename)

    # Regex:
    #   (?=...) positive lookahead so overlapping codons can be matched
    #   (AUG(?:...(?!UAA|UAG|UGA))*) captures start AUG and all following codons
    #   stops before first stop codon
    codon_regex = re.compile(r'(AUG(?:(?!UAA|UAG|UGA)...)*)(?=UAA|UAG|UGA|$)')

    with open(output_file, 'w') as file:
        while True:
            seq_input = input("SEQUENCE> ")
            if seq_input == "DONE":
                break

            # Header of output line
            output = seq_input + " "

            # Convert DNA → RNA
            sequence = dna_to_rna(seq_input)

            match = codon_regex.search(sequence)
            if not match:
                file.write(output + "\n")
                continue

            coding_region = match.group(1)

            # Break into 3-letter codons
            codons = [coding_region[i:i+3] for i in range(0, len(coding_region), 3)]

            # Translate codons using acids list
            for codon in codons:
                for acid in acids:
                    if codon == acid[0]:
                        output += acid[2]

            file.write(output + "\n")
