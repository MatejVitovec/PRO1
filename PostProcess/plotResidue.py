import sys
from matplotlib import pyplot as plt

inputFileName = "PostProcess/residue.txt"
inputFileName2 = "PostProcess/residue2ord.txt"
inputFileName3 = "PostProcess/residueFine.txt"
outputFileName = "PostProcess/residueResult.png"

f = open(inputFileName, "r")
res = []

step = int(f.readline())

for line in f:
    res.append(float(line))

f.close()

f = open(inputFileName2, "r")
res2 = []

step2 = int(f.readline())

for line in f:
    res2.append(float(line))

f.close()

f = open(inputFileName3, "r")
res3 = []

step3 = int(f.readline())

for line in f:
    res3.append(float(line))

f.close()

plt.figure()

plt.plot(range(0, len(res)*step, step), res, "r-")
plt.plot(range(0, len(res2)*step2, step2), res2, "g-")
plt.plot(range(0, len(res3)*step3, step3), res3, "k-")
plt.yscale("log")
plt.ylabel("||rho||")
plt.xlabel("Iter")

plt.tight_layout()
plt.savefig(outputFileName)

print("Byl vytvořen soubor ", outputFileName)

plt.close()