#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Lab 1A
#   References: 
#   Time: 20 minutes

def CalcMixedFraction(Num, Denom):
    WholeNumber = Num // Denom
    FractionNumber = Num % Denom
    return print("OUTPUT", WholeNumber, f"{FractionNumber}" '/' f"{Denom}")

print("Input the numerator of the improper fraction.")
Numerator = int(input("NUMERATOR> "))
print("Input the denominator of the improper fraction.")
Denominator = int(input("DENOMINATOR> "))
print(f"{Numerator}" '/' f"{Denominator}" ' as a mixed fraction is:')
CalcMixedFraction(Numerator, Denominator)
