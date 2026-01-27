import math

W = [-0.0331, 0.1468, -0.2819, 1.1681]
SPIKE_THRESHOLD = 100
TSUNAMI_THRESHOLD = 30

buoy1 = [float(val) for val in input("Buoy #1 - OBERSERVED PRESSURES (mm)> ").split()]
buoy2 = [float(val) for val in input("Buoy #2 - OBERSERVED PRESSURES (mm)> ").split()]

b1_predict_pressure = (W[0] * buoy1[0]) + (W[1] * buoy1[1]) + (W[2] * buoy1[2]) + (W[3] * buoy1[3])
b2_predict_pressure = (W[0] * buoy2[0]) + (W[1] * buoy2[1]) + (W[2] * buoy2[2]) + (W[3] * buoy2[3])

b1_pressure_diff = abs(buoy1[4] - b1_predict_pressure)
b2_pressure_diff = abs(buoy2[4] - b2_predict_pressure)

print(f"OUTPUT Buoy #1: Pressure difference of {b1_pressure_diff:.2f} mm")
print(f"OUTPUT Buoy #2: Pressure difference of {b2_pressure_diff:.2f} mm")

if (b1_pressure_diff > SPIKE_THRESHOLD) or (b2_pressure_diff > SPIKE_THRESHOLD):
    print(f"OUTPUT Spike detected. Ignoring data")

elif (b1_pressure_diff < SPIKE_THRESHOLD) and (b2_pressure_diff < SPIKE_THRESHOLD):
    if (b1_pressure_diff >= TSUNAMI_THRESHOLD) and (b1_pressure_diff >= TSUNAMI_THRESHOLD):
        pass

    else:
        if (b1_pressure_diff > TSUNAMI_THRESHOLD):
            pass
        else:
            pass
        if (b2_pressure_diff > TSUNAMI_THRESHOLD):
            print(f"OUTPUT Buoy #2: Tsunami wave detected")
        else:
             print(f"OUTPUT Buoy #2: No abnormal waves detected")
