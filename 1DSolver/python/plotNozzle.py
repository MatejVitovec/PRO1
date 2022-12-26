import sys
from matplotlib import pyplot as plt

sys.argv = "", "MUSCL-Hancock/results/case1.txt", "-l", "MUSCL-Hancock/results/case1.png"

def plotMultipleCurves(x, y, plotType):
    for i in range(len(x)):
        plotStyle = "k."

        if (plotType[i] == "-l"):
            plotStyle = "k-"

        plt.plot(x[i], y[i], plotStyle)


inputFileNum = 1

if len(sys.argv) > 1:
    inputFileNum = (int)(len(sys.argv)/2 - 1)

inputFileName = [0] * inputFileNum
plotType = [0] * inputFileNum

firstX = [0] * inputFileNum
dx = [0] * inputFileNum
cells = [0] * inputFileNum
time = [0] * inputFileNum
d = [0] * inputFileNum
u = [0] * inputFileNum
p = [0] * inputFileNum
ma = [0] * inputFileNum
x = [0] * inputFileNum

inputFileName[0] = "results/result.txt"
outputFileName = "results/result.png"
plotType[0] = "-l"

f = [0] * inputFileNum


for i in range(0, inputFileNum):
    if len(sys.argv) > 1:
        inputFileName[i] = sys.argv[i*2 + 1]
        plotType[i] = sys.argv[i*2 + 2]

    f[i] = open(inputFileName[i], "r")

    firstX[i] = float(f[i].readline())
    dx = float(f[i].readline())
    cells[i] = int(f[i].readline())
    time[i] = float(f[i].readline())
    

    d[i] = []
    u[i] = []
    p[i] = []
    ma[i] = []
    x[i] = []

    for line in f[i]:
        aux = line.split(",")
        d[i].append(float(aux[0]))
        u[i].append(float(aux[1]))
        p[i].append(float(aux[2]))
        ma[i].append(float(aux[3]))

    for j in range(0, cells[i]):
        x[i].append(j*dx + firstX[i])
        j = j + 1

    f[i].close

for t in time:
    if time[0] != t:
        print("Vstupní data nemají stejný čas")
        exit() 

outputFileName = sys.argv[inputFileNum*2 + 1]

plt.figure()

plt.subplot(2,2,1)
plotMultipleCurves(x, d, plotType)
plt.xlabel("Position")
plt.ylabel("Density")

plt.subplot(2,2,2)
plotMultipleCurves(x, u, plotType)
plt.xlabel("Position")
plt.ylabel("Velocity")

plt.subplot(2,2,3)
plotMultipleCurves(x, p, plotType)
plt.xlabel("Position")
plt.ylabel("Pressure")

plt.subplot(2,2,4)
plotMultipleCurves(x, ma, plotType)
plt.xlabel("Position")
plt.ylabel("Mach number")

plt.suptitle("t = " + str(time[0]))

plt.tight_layout()

plt.savefig(outputFileName)

print("Byl vytvořen soubor ", outputFileName)

plt.close()