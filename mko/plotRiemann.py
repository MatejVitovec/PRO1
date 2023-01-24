import sys
from matplotlib import pyplot as plt

sys.argv = "", "mko/results/sod2Exact.txt", "k-", "mko/results/sod2HLL.txt", "g-o", "mko/results/sod2muscl.txt", "r-o", "mko/results/sod2muscl.png"

def plotMultipleCurves(x, y, plotType):
    for i in range(len(x)):
#        plotStyle = "k."

#        if (plotType[i] == "-l"):
#           plotStyle = "k-"
        plotStyle = plotType[i]

        plt.plot(x[i], y[i], plotStyle, markersize = 3)
    a = 5


inputFileNum = 1

if len(sys.argv) > 1:
    inputFileNum = (int)(len(sys.argv)/2 - 1)

inputFileName = [0] * inputFileNum
plotType = [0] * inputFileNum

domLen = [0] * inputFileNum
cells = [0] * inputFileNum
time = [0] * inputFileNum
d = [0] * inputFileNum
u = [0] * inputFileNum
p = [0] * inputFileNum
e = [0] * inputFileNum
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

    domLen[i] = float(f[i].readline())
    cells[i] = int(f[i].readline())
    time[i] = float(f[i].readline())
    dx = domLen[i]/cells[i]

    d[i] = []
    u[i] = []
    p[i] = []
    e[i] = []
    x[i] = []

    for line in f[i]:
        aux = line.split(",")
        d[i].append(float(aux[0]))
        u[i].append(float(aux[1]))
        p[i].append(float(aux[2]))
        e[i].append(float(aux[3]))

    for j in range(0, cells[i]):
        x[i].append(j*dx)
        j = j + 1

    f[i].close

for t in time:
    if time[0] != t:
        print("Vstupní data nemají stejný čas")
        exit() 

outputFileName = sys.argv[inputFileNum*2 + 1]

#plt.figure()
plt.figure(figsize=(8,6))

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
plotMultipleCurves(x, e, plotType)
plt.xlabel("Position")
plt.ylabel("Internal energy")

plt.suptitle("t = " + str(time[0]))

plt.tight_layout()

plt.savefig(outputFileName, dpi = 300)
#plt.savefig("output.pdf")

print("Byl vytvořen soubor ", outputFileName)

plt.close()