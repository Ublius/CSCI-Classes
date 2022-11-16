#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Lab 2
#   References: 
#   Time: 15 minutes

#Get words to encrypt
print("Input the five lists of characters to be encrypted.")
List1 = str(input("LIST1> "))
List2 = str(input("LIST2> "))
List3 = str(input("LIST3> "))
List4 = str(input("LIST4> "))
List5 = str(input("LIST5> "))

#Slice the words
FirstWord = List1[-2:] + List1[0:-2]
SecondWord = List2[0:-2]
ThirdWord = List3[round(len(List3)/2):]
FourthWord = List4[0:2] + List4[4] + List4[3] + List4[2] + List4[5:]
print(f'OUTPUT {List5[:2]} {FirstWord}{SecondWord}{ThirdWord}{FourthWord} {List5[2:]}')