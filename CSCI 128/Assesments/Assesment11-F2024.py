import matplotlib.pyplot as plt

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

def create_amino_hist(plot_name, aminos):
    plt.hist(aminos)
    plt.title("Histogram of Amino Acids")
    plt.xlabel("Amino Acid Abbreviations")
    plt.ylabel("Counts")
    plt.savefig(plot_fileName)

def create_gc_scatter(plot_name, gc_ratios, sequence_lengths):
    plt.scatter(gcs, lengths)
    plt.title("Scatterplot of Sequence Length vs GC Content")
    plt.xlabel("GC Content Ratio")
    plt.ylabel("Sequence Length")
    plt.savefig(plot_filename)

def create_base_lineplot(plot_name, sequence):
    pass
    
if __name__ == "__main__":
    codon_fileName = input("CODONS_FILENAME> ")
    sequence_fileName = input("SEQUENCES_FILENAME> ")

    print("What plot would you like to generate?\n \t1: Codons Histogram\n \t2: GC percentage scatterplot\n \t3: Acid plot")

    choice = input("PLOT#> ")
    plot_fileName = input("PLOTNAME> ")

    if str(choice) == "1":
        acids = parse_file_into_acids(codon_fileName)
        create_amino_hist(plot_fileName, acids)

    elif str(choice) == "2":
        create_gc_scatter()

    elif str(choice) == "3":
        seq_num = input("SEQ_NUM> ")
        with open(sequence_fileName, 'r') as file:
            sequence = file.readline().strip()
        rna_sequence = dna_to_rna(sequence[seq_num])
        create_base_lineplot()
