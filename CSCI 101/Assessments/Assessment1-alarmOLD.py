#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Assessment 1
#   References: no one
#   Time: 1 hour 45 min

# Variables
hourTF = 0
hourSub = 0
minSub = 0


# Get how many mins they would like to wake up early
print("How many minutes early would you like your alarm to go off?")
minsEarly = int(input("EARLY> "))

# Get what time they would like to get out of bed
print("What time do you need to get out of bed?")
hour = float(input("HOURS> "))
min = int(input("MINUTES> "))

# Calculate Hour
hourTF = (hour - (minsEarly / 60))
if hourTF < 0:
    hourSub = int(24 + (hourTF -0.5))
elif (minsEarly // 60) == 0 and minsEarly <= min:
    hourSub = hour
else:
    hourSub = hourTF

# Calculate Min
if minsEarly > 60 and (minsEarly % 2) == 0:
    minSub = abs(min - (minsEarly - ((minsEarly // 60) * 60) + min))
    print(minSub)
elif minsEarly > 60 and (minsEarly % 30) != (minsEarly % 2):
    minSub = abs(min - (minsEarly - ((minsEarly // 60) * 60)))
elif (minsEarly % 60) == 0:
    minSub = min
elif minsEarly > min:
    minSub = abs((minsEarly - 60)) + min
elif minsEarly < 60 and minsEarly < min:
    minSub = min - minsEarly

# Ouput time to set alarm
print(f'OUTPUT {hourSub:02} {minSub:02}')
# if hourSub < 10 and minSub < 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT 0{hourSub:.0f} 0{minSub:.0f}')
# elif hourSub >= 10 and minSub >= 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT {hourSub:.0f} {minSub:.0f}')
# elif hourSub > 10 and minSub < 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT 0{hourSub:.0f} 0{minSub:.0f}')
# elif hourSub >= 10 and minSub < 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT {hourSub:.0f} {minSub:.0f}')
# elif hourSub < 10 and minSub < 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT 0{hourSub:.0f} 0{minSub:.0f}')
# elif hourSub < 10 and minSub >= 10:
#     print("You should set your alarm for:")
#     print(f'OUTPUT 0{hourSub:.0f} {minSub:.0f}')
