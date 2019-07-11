import os


def readFile(fileName):

    #Defining variables for text formatting
    lineCount = 0

    # Creates the new gCode Output file (.txt)
    with open("gCodeOutput.txt","w") as newGcode:

        # Opens the original gCode (unformatted)
        with open(fileName,'r') as gCode:

            #Devides the line by white spaces into a list
            for line in gCode:
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

readFile("bottom_contour.txt")
