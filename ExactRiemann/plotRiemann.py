import sys
from matplotlib import pyplot as plt

inputFileName = "ExactRiemann/results/result.txt"
outputFileName = "ExactRiemann/results/result.png"

if len(sys.argv) == 3:
    inputFileName = sys.argv[1]
    outputFileName = sys.argv[2]

f = open(inputFileName, "r")

domLen = float(f.readline())
cells = int(f.readline())
time = float(f.readline())

dx = domLen/cells

d = []
u = []
p = []
e = []
x = []

for line in f:
    aux = line.split(",")
    d.append(float(aux[0]))
    u.append(float(aux[1]))
    p.append(float(aux[2]))
    e.append(float(aux[3]))

f.close()

for i in range(0, cells + 1):
    x.append(i*dx)
    i = i + 1


plt.figure()

plt.subplot(2,2,1)
plt.plot(x,d)
plt.xlabel("Position")
plt.ylabel("Density")

plt.subplot(2,2,2)
plt.plot(x,u)
plt.xlabel("Position")
plt.ylabel("Velocity")

plt.subplot(2,2,3)
plt.plot(x,p)
plt.xlabel("Position")
plt.ylabel("Pressure")

plt.subplot(2,2,4)
plt.plot(x,e)
plt.xlabel("Position")
plt.ylabel("Internal energy")

plt.suptitle("t = " + str(time))

plt.tight_layout()

plt.savefig(outputFileName)

plt.close()