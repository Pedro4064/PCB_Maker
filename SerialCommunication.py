import serial

port = '/dev/tty.usbmodem14201'

ser = serial.Serial(port,9600)
print("The port you are connected is: ",ser.name)
ser.write(b'Z123.000')

# with open("gCodeOutput.txt",'r') as gCode:
#
#     for line in gCode:
#         print(line.strip('\n'))
#         ser.write(line.strip('\n').encode())

ser.close()
