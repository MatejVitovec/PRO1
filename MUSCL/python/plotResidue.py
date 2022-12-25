import sys
from matplotlib import pyplot as plt

sys.argv = "", "Nozzle/results/case1Res.txt", "Nozzle/results/case1Res.png"

if len(sys.argv) > 1:
    inputFileName = sys.argv[1]

f = open(inputFileName, "r")
res = []

dIter = float(f.readline())


for line in f:
    res.append(float(line))


outputFileName = sys.argv[2]

plt.figure()

plt.plot([*range(0, len(res)*int(dIter), int(dIter))], res)
plt.ylabel("density res")
plt.xlabel("Iter")

plt.tight_layout()
plt.savefig(outputFileName)

print("Byl vytvořen soubor ", outputFileName)

plt.close()