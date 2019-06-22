import serial
import time

#Declare the port the arduino is in
port = '/dev/tty.usbmodem14201'

#specify the port and the boundrate (match the arduino's baund rate specified in the Serial.begin())
ser = serial.Serial(port,baudrate = 9600, timeout = 3)
print("The port you are connected is: ",ser.name)

# ser.write(b'Z123.000')

next = False

#Opens the gCode file
with open("gCodeOutput.txt",'r') as gCode:

    #Goes line by line
    for line in gCode:

        print(line.strip('\n'))
        #Send to the arduino the line
        ser.write(line.strip('\n').encode())

        # Need to chakc how long it takes from one corner to the next (or use the gpios of the π to receive a HIGH to continue )
        time.sleep(3)




ser.close()
