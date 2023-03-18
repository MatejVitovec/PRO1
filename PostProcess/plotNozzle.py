import sys
from matplotlib import pyplot as plt

#sys.argv = "", "PostProcess/nozzle50GodunovHllc.txt", "r.-", "PostProcess/nozzle50MinmodHllc.txt", "gx-", "PostProcess/nozzle50VanAlbadaHllc.txt", "b.-", "PostProcess/nozzle50SuperbeeHllc.txt", "k.-", "PostProcess/nozzle50MCHllc.txt", "y.-", "PostProcess/nozzle50HllcDetail.png"
sys.argv = "", "PostProcess/nozzle50GodunovHllc.txt", "r.-", "PostProcess/nozzle200GodunovHllc.txt", "g.-", "PostProcess/nozzle500GodunovHllc.txt", "b.-", "PostProcess/nozzle500Hllc.png"
#sys.argv = "", "PostProcess/nozzle50GodunovHll.txt", "r.-", "PostProcess/nozzle50GodunovHllc.txt", "g.-", "PostProcess/nozzle50MinmodHll.txt", "b.-", "PostProcess/nozzle50MinmodHllc.txt", "k.-", "PostProcess/nozzle50HllHllc.png"



def plotMultipleCurves(x, y, plotType):
    for i in range(len(x)):
        plt.plot(x[i], y[i], plotType[i], linewidth=0.7, markersize=2.0)

def plotMultipleCurvesl(x, y, plotType, labels):
    for i in range(len(x)):
        plt.plot(x[i], y[i], plotType[i], linewidth=0.7, markersize=2.0, label = labels[i])


inputFileNum = 1

if len(sys.argv) > 1:
    inputFileNum = (int)(len(sys.argv)/2 - 1)

labels =  [0] * inputFileNum
labels[0] = "n = 50"
labels[1] = "n = 200"
labels[2] = "n = 500"
#labels[3] = "Minmod HLLC"
#labels[4] = "MC"

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
    firstX[i] = firstX[i] + dx/2.0
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
        #exit() 

outputFileName = sys.argv[inputFileNum*2 + 1]

plt.figure(figsize=(8,6))

plt.subplot(2,2,1)
plotMultipleCurvesl(x, d, plotType, labels)
plt.xlabel("Position")
plt.ylabel("Density")
plt.legend()
#plt.xlim([0.6,0.8])
plt.grid()

plt.subplot(2,2,2)
plotMultipleCurves(x, u, plotType)
plt.xlabel("Position")
plt.ylabel("Velocity")
#plt.xlim([0.6,0.8])
plt.grid()

plt.subplot(2,2,3)
plotMultipleCurves(x, p, plotType)
plt.xlabel("Position")
plt.ylabel("Pressure")
#plt.xlim([0.6,0.8])
plt.grid()

plt.subplot(2,2,4)
plotMultipleCurves(x, ma, plotType)
plt.xlabel("Position")
plt.ylabel("Mach number")
#plt.xlim([0.6,0.8])
plt.grid()

plt.suptitle("iter = " + str(time[0]) + ", cells = " + str(cells[0]))


plt.tight_layout()

plt.savefig(outputFileName, dpi = 200)

print("Byl vytvořen soubor ", outputFileName)

plt.close()