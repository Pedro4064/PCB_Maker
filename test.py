from PyQt5.QtWidgets import QMainWindow, QAction, qApp, QApplication, QPushButton, QToolBar, QTableWidget,QTableWidgetItem, QInputDialog, QLineEdit, QFileDialog,QLabel,QMessageBox,QProgressBar
from PyQt5.QtGui import QIcon, QPixmap, QImage
import matplotlib.pyplot as plt
import serial
import time
import sys


class App(QMainWindow,QPushButton, QToolBar, QIcon, QTableWidget, QTableWidgetItem, QLabel, QPixmap, QImage,QFileDialog,QMessageBox,QProgressBar):

    def __init__(self):
        super().__init__()
        self.title = 'gCode Plotter'
        self.left = 0
        self.top = 0
        self.width = 1280
        self.height = 960

        self.fileName = " "
        self.lastFile = " "

        self.serialPort = " "

        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)


        FormatBtn = QPushButton("Format gCode",self)
        FormatBtn.move(0,0)
        FormatBtn.resize(1280,100)


        chooseFileBtn = QPushButton('Upload file', self)
        chooseFileBtn.move(0,100)
        chooseFileBtn.resize(1280,100)


        lastFileBtn = QPushButton('Upload Last file',self)
        lastFileBtn.move(0,200)
        lastFileBtn.resize(1280,100)

        portBtn = QPushButton('Set serial port for the arduino',self)
        portBtn.move(0,300)
        portBtn.resize(1280,100)

        # sendSerialBtn = QPushButton('Send serial data to the arduino',self)
        # sendSerialBtn.move(0,400)
        # sendSerialBtn.resize(1280,100)

        self.ProgressBar = QProgressBar(self)
        self.ProgressBar.setGeometry(400, 400, 400, 400)
        self.ProgressBar.move(450,400)

        self.label  =QLabel(self)
        self.label.move(625,600)
        self.label.setText("Progress Bar")


        chooseFileBtn.clicked.connect(self.openFileNameDialog)
        lastFileBtn.clicked.connect(self.PlotLast)
        FormatBtn.clicked.connect(self.FormatGcode)
        portBtn.clicked.connect(self.setSerialPort)


        self.show()

    def openFileNameDialog(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        self.fileName, ok = QFileDialog.getOpenFileName(self,"QFileDialog.getOpenFileName()", "","Text Files (*.txt)", options=options)


        if self.fileName:

            # Update the lastFile path, so the user can press the lastFileBtn and open the las used file
            self.lastFile = self.fileName
            print(self.fileName)
            self.Plot()

            buttonReply = QMessageBox.question(self, 'Start Serial Connection', "Sure you want to print this PCB?", QMessageBox.Yes | QMessageBox.No, QMessageBox.No)

            if buttonReply == QMessageBox.Yes:
                print('Uploading via serial ')
                self.SendSerial(self.fileName)

            else:
                print('No clicked.')

    def Plot(self):
        try:

            x = [None]
            x= []

            y = [None]
            y=[]

            plot = True

            # Open the gCode file
            with open(self.fileName,"r") as gcode:

                # Read line by line
                for line in gcode:

                    # If it starts with a Z, dont append it to the x and y lists
                    if 'Z' in line:

                        # line = line.strip('Z').strip("\n")
                        # z = float(line)
                        #
                        # # Don't add to the "coordinate to the plotted" the coordinates that have the pen off the plate
                        # if z>0:
                        #     plot = False
                        #
                        # elif z<= 0:
                        #     plot = True
                        continue


                    # Add the x and y coordinates
                    if plot == True:
                        line = line.split(" ")
                        x.append(float(line[0].strip('X')))
                        y.append(float(line[1].strip('Y').strip("\n")))


                # Plot all the coordinates and set the settings
                # plt.figure(figsize = (10,5))
                plt.plot(x,y)
                plt.title("Pen Path")
                plt.xlabel("mm")
                plt.ylabel("mm")

                # Show the picture
                plt.show()
        except:
            print("fuck")

    def PlotLast(self):
        try:
            x = [None]
            x= []

            y = [None]
            y=[]

            plot = True

            # Open the gCode file
            with open(self.lastFile,"r") as gcode:

                # Read line by line
                for line in gcode:

                    # If it starts with a Z, dont append it to the x and y lists
                    if 'Z' in line:

                        # line = line.strip('Z').strip("\n")
                        # z = float(line)
                        #
                        # # Don't add to the "coordinate to the plotted" the coordinates that have the pen off the plate
                        # if z>0:
                        #     plot = False
                        #
                        # elif z<= 0:
                        #     plot = True
                        continue


                    # Add the x and y coordinates
                    if plot == True:
                        line = line.split(" ")
                        x.append(float(line[0].strip('X')))
                        y.append(float(line[1].strip('Y').strip("\n")))


                # Plot all the coordinates and set the settings
                # plt.figure(figsize = (10,5))
                plt.plot(x,y)
                plt.title("Pen Path")
                plt.xlabel("mm")
                plt.ylabel("mm")

                # Show the picture
                plt.show()
        except:
            print("Fuck")

    def FormatGcode(self):

        ########################Get the File to be formatted####################
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        self.gCodeFile, ok = QFileDialog.getOpenFileName(self,"QFileDialog.getOpenFileName()", "","Text Files (*.txt)", options=options)

        # Creates the QProgressBar

        self.ProgressBar.setRange(0,self.getNumberOfLines(self.gCodeFile))
        self.label.setText("formatting file ...")



        if self.gCodeFile:

            buttonReply = QMessageBox.question(self, 'gCode formatting', "Sure you want to format this gcode?", QMessageBox.Yes | QMessageBox.No, QMessageBox.No)

            if buttonReply == QMessageBox.Yes:
                print('Yes clicked.')

            #############################format the gcode#######################
            #Defining variables for text formatting
            lineCount = 0

            # Get the name of the file to be formatted, get rid of the path and add "output" to the name of the new file
            name = self.gCodeFile.split('/')
            name = "output_"+name[len(name)-1]

            # Creates the new gCode Output file (.txt)
            with open(name,"w") as newGcode:

                # Opens the original gCode (unformatted)
                with open(self.gCodeFile,'r') as gCode:

                    # Get the number of the line it is in
                    count = 0

                    #Devides the line by white spaces into a list
                    for line in gCode:

                        #update the ProgressBar
                        count+=1
                        self.ProgressBar.setValue(count)


                        if "(" in line:
                            continue
                        line = line.split(" ")

                        #goes through each  section of the line
                        for section in line:

                            #If it has z,y or z it writes it to the new file
                            if "X" in section or "Z" in section or "Y" in section:

                                #Formats the new file by adding space between z/x/y info
                                if lineCount>0:
                                    newGcode.write(section.replace("Y"," Y"))
                                else:
                                    newGcode.write(section)

                                lineCount+=1

                            else:
                                continue

                self.label.setText("Done")
                self.setGeometry(self.left, self.top, self.width, self.height)

    def setSerialPort(self):

        ports = ['/dev/tty.usbmodem14101','/dev/tty.usbmodem14201']
        device, ok = QInputDialog.getItem(self, 'Device', 'Choose your device', ports,0,False)
        if ok == True:
            # print(device)
            self.serialPort = device

    def SendSerial(self,file):
        print("oi")

        # if no port is defined, go to the setSerialPort and then continue
        if self.serialPort == " ":
            self.setSerialPort()


        print("Sending serial info")
        print(file)


    def getNumberOfLines(self,name):
        count = 0
        with open(name,'r') as file:
            for line in file:
                count+=1
        return count
if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    sys.exit(app.exec_())
