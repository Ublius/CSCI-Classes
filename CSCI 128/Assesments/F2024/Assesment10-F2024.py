def dna_to_rna(sequence):
    conversions = {'A': 'U', 'T': 'A', 'C': 'G', 'G': 'C'}
    rna = ''
    for nucleotide in sequence:
        rna += conversions[nucleotide]
    return rna


def parse_file_into_acids(filename):
    filelist = []
    with open(filename) as myfile:
        for row in myfile:
            newrow = row.split()
            filelist.append(newrow)
    return filelist

"""
def test_my_functions():
    assert dna_to_rna('ATCG') == 'UAGC'
    assert dna_to_rna('ATTCCAGTC') == 'UAAGGUCAG'
    assert dna_to_rna('CCGGGCCCGGCTACCGACCCATGATTAAACCCTACTCAAA') == 'GGCCCGGGCCGAUGGCUGGGUACUAAUUUGGGAUGAGUUU'

    assert parse_file_into_acids('test.txt') == [["AAA", "Lys", "K", "Lysine"],
                                                ["AAC", "Asn", "N", "Asparagine"],
                                                ["AAG", "Lys", "K" , "Lysine"]]
"""

if __name__ == '__main__':
    codonsFilename = input("CODON FILENAME> ")
    sequenceFilename = input("SEQUENCE FILENAME> ")
    output_file = input("OUTPUT FIlE> ")

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

                # find where "AUG" is in the sequence
                start = sequence.find(start_codon)
                # record all codons until the stop codon
                codons = ["AUG"]
                for i in range(start + 3, len(sequence), 3):
                    codon = sequence[i:i+3]
                    if codon in stop_codons:
                        break
                    codons.append(codon)
                # print(codons)
                # convert codons to amino acid single letter abbreviations

                for codon in codons:
                    for acid in acids:
                        if codon == acid[0]:
                            output += acid[2]

                file.write(output)
                file.write("\n")
