import minimalmodbus
import serial
from time import sleep

#python3 -m venv venv
#source venv/bin/activate

# Hardware parameters:
instrument = minimalmodbus.Instrument('/dev/ttyUSB0', 1)    # Replace COM3 with your actual port.
                                                    # Check ports on Device Manager to match the device.
instrument.serial.baudrate = 9600
instrument.serial.bytesize = 8
instrument.serial.parity = serial.PARITY_NONE
instrument.serial.stopbits = 1
instrument.serial.timeout = 1.0
instrument.mode = minimalmodbus.MODE_RTU

# Sensor data:
# try:
#while True:
salinity = instrument.read_register(20, 0, 3, signed=False)
conductivity = salinity / 640.0 # EC(dS/m) = TDS(mg/L)÷640
                                # Electrical Conductivity (EC) = Total Disolved Solids (TDS)

                                # Val. for natural water:           ~640.0
                                # Val. for seawater:                ~530.0
                                # Val. for agricultural drainage:   ~700.0

print(f"\nEstimated Soil Readings:")
print(f"Salinity: {salinity:.3f} mg/L")
print(f"Conductivity: {conductivity:.3f} dS/m \n")
sleep(1)

# except Exception as e:
#     print(f"Error reading sensor: {e}")