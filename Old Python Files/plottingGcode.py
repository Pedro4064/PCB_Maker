import matplotlib.pyplot as plt

x = [None]
x= []

y = [None]
y=[]

plot = True

# Open the gCode file
with open("gCodeOutput.txt","r") as gcode:

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
    plt.plot(x,y)
    plt.title("Pen Path")
    plt.xlabel("mm")
    plt.ylabel("mm")

    # Show the picture
    plt.show()
