import math

W = [-0.0331, 0.1468, -0.2819, 1.1681]
SPIKE_THRESHOLD = 100
TSUNAMI_THRESHOLD = 30
EARTH_RADIUS = 6371

buoy1 = [float(val) for val in input("Buoy #1 - OBSERVED PRESSURES (mm)> ").split()]
buoy2 = [float(val) for val in input("Buoy #2 - OBSERVED PRESSURES (mm)> ").split()]

b1_predict_pressure = (W[0] * buoy1[0]) + (W[1] * buoy1[1]) + (W[2] * buoy1[2]) + (W[3] * buoy1[3])
b2_predict_pressure = (W[0] * buoy2[0]) + (W[1] * buoy2[1]) + (W[2] * buoy2[2]) + (W[3] * buoy2[3])

b1_pressure_diff = abs(buoy1[4] - b1_predict_pressure)
b2_pressure_diff = abs(buoy2[4] - b2_predict_pressure)

print(f"OUTPUT Buoy #1: Pressure difference of {b1_pressure_diff:.2f} mm")
print(f"OUTPUT Buoy #2: Pressure difference of {b2_pressure_diff:.2f} mm")

if (b1_pressure_diff > SPIKE_THRESHOLD) or (b2_pressure_diff > SPIKE_THRESHOLD):
    print(f"OUTPUT Spike detected. Ignoring data")

elif (b1_pressure_diff < SPIKE_THRESHOLD) and (b2_pressure_diff < SPIKE_THRESHOLD):
    if (b1_pressure_diff >= TSUNAMI_THRESHOLD) and (b2_pressure_diff >= TSUNAMI_THRESHOLD):
        buoy1_cords = [float(val) for val in input("Buoy #1 - COORDINATES> ").split(",")]
        buoy2_cords = [float(val) for val in input("Buoy #2 - COORDINATES> ").split(",")]
        time = float(input("TIME BETWEEN WAVES (sec)> "))

        b1_lat, b1_lon = buoy1_cords[0] * (math.pi / 180), buoy1_cords[1] * (math.pi / 180)
        b2_lat, b2_lon = buoy2_cords[0] * (math.pi / 180), buoy2_cords[1] * (math.pi / 180)

        d_lat = b2_lat - b1_lat
        d_lon = b2_lon - b1_lon

        a = math.sin(d_lat / 2)**2 + math.cos(b1_lat) * math.cos(b2_lat) * math.sin(d_lon / 2)**2
        c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
        distance = EARTH_RADIUS * 1000 * c

        wave_speed = distance / time if time > 0 else 0

        print(f"OUTPUT Wave speed: {wave_speed:.2f} m/s")

    else:
        if (b1_pressure_diff > TSUNAMI_THRESHOLD):
            print(f"OUTPUT Buoy #1: Tsunami wave detected")
        else:
            print(f"OUTPUT Buoy #1: No abnormal waves detected")
        if (b2_pressure_diff > TSUNAMI_THRESHOLD):
            print(f"OUTPUT Buoy #2: Tsunami wave detected")
        else:
             print(f"OUTPUT Buoy #2: No abnormal waves detected")