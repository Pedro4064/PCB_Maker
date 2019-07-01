import serial
import time

#Declare the port the arduino is in
port = '/dev/tty.usbmodem14101'

#specify the port and the boundrate (match the arduino's baund rate specified in the Serial.begin())
ser = serial.Serial(port,baudrate = 9600)
print("The port you are connected is: ",ser.name)

next = False


def sendCommands(commands):
    ser.write(commands)



#Opens the gCode file
with open("gCodeOutput.txt",'r') as gCode:


    print('Starting to send gcode...')

    #Goes line by line
    for line in gCode:

        time.sleep(5) #Perhaps we wont need this delay since the arduino will take his time to move the motors...
        next = False

        print(line.strip('\n'))

        #Send to the arduino the line
        sendCommands(line.strip('\n').encode())

        # Need to chakc how long it takes from one corner to the next (or use the gpios of the π to receive a HIGH to continue )
        while next == False:

            # print('.')
            try:
                if ser.read().decode('ascii') == 'N':
                    next = True
                    print('sending next command')
                    ser.write(b'ok')

                    break
            except:
                continue




ser.close()
