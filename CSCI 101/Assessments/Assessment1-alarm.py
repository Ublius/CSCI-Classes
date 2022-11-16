#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Assessment 1
#   References: no one
#   Time: 1 hour

# Get how many mins they would like to wake up early
print("How many minutes early would you like your alarm to go off?")
minsEarly = int(input("EARLY> "))

# Get what time they would like to get out of bed
print("What time do you need to get out of bed?")
T2WH = int(input("HOURS> "))
T2WM = int(input("MINUTES> "))

# Convert T2WH & T2WM into seconds || Convert minsEarly into seconds
T2WSeconds = (T2WH * 60 * 60) + (T2WM * 60)
mEseconds = minsEarly * 60

# Subtract T2WSeconds by mEseconds
TotalSeconds = T2WSeconds - mEseconds

# Calculate Hour and Minute
Hour = TotalSeconds // (60 * 60)
remaningSeconds = TotalSeconds - (Hour * 60 * 60)
Minutes = int(remaningSeconds / 60)

# Adjust for 24-HR Clock
if Hour < 0:
    Hour = 24 + Hour

# Ouput time to set alarm
print(f'OUTPUT {Hour:02d} {Minutes:02d}')
