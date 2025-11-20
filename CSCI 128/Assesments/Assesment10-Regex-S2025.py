import re


# -------------------------------
# Convert DNA to RNA using regex
# -------------------------------
def dna_to_rna(sequence):
    # Replace each nucleotide using a regex substitution callback
    conversions = {'A': 'U', 'T': 'A', 'C': 'G', 'G': 'C'}

    def replace(match):
        base = match.group(0)
        return conversions[base]

    return re.sub(r'[ATCG]', replace, sequence)


# -------------------------------
# Parse file into acids (unchanged)
# -------------------------------
def parse_file_into_acids(filename):
    filelist = []
    with open(filename) as myfile:
        for row in myfile:
            newrow = row.split()
            filelist.append(newrow)
    return filelist


# -------------------------------------------
#   MAIN PROGRAM (codon scanning via regex)
# -------------------------------------------
if __name__ == '__main__':
    codonsFilename = input("CODON FILENAME> ")
    sequenceFilename = input("SEQUENCE FILENAME> ")
    output_file = input("OUTPUT FILE> ")

    acids = parse_file_into_acids(codonsFilename)

    start_codon = "AUG"
    stop_codons = ["UAA", "UGA", "UAG"]

    # Create a regex that matches:
    # start codon AUG followed by any number of codons (3 letters each)
    codon_regex = re.compile(r'(AUG(?:...)*?)')

    with open(output_file, 'w') as file:
        with open(sequenceFilename, 'r') as myfile:
            while True:
                sequence = myfile.readline().strip()
                output = sequence + " "

                if sequence == "DONE":
                    break

                # Convert DNA → RNA using REGEX
                sequence = dna_to_rna(sequence)

                # -------------------------------------
                # Find start codon and all codons via regex
                # -------------------------------------
                # Find AUG and everything after in groups of 3
                match = re.search(r'AUG(?:...)*', sequence)

                if not match:
                    # No valid start codon found
                    file.write(output + "\n")
                    continue

                full_codon_region = match.group(0)

                # Split into codons of length 3
                codons = re.findall(r'...', full_codon_region)

                # Stop at stop codon
                final_codons = []
                for c in codons:
                    if c in stop_codons:
                        break
                    final_codons.append(c)

                # -------------------------------------
                # Convert codons → amino acids
                # -------------------------------------
                for codon in final_codons:
                    for acid in acids:
                        if codon == acid[0]:
                            output += acid[2]  # single-letter abbreviation

                file.write(output + "\n")
