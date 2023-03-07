import sys
from matplotlib import pyplot as plt

inputFileName = "PostProcess/residueNew.txt"
inputFileName2 = "PostProcess/residueMinmod.txt"
inputFileName3 = "PostProcess/residueMinmodHllc.txt"

inputFileName4 = "PostProcess/residueNew.txt"
inputFileName5 = "PostProcess/residueNew.txt"



#inputFileName2 = "PostProcess/residueMinmod.txt"
#inputFileName3 = "PostProcess/residueVanAlbada.txt"
#inputFileName4 = "PostProcess/residueSuperbee.txt"
#inputFileName5 = "PostProcess/residueMC.txt"
outputFileName = "PostProcess/residue.png"


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



f = open(inputFileName4, "r")
res4 = []

step4 = int(f.readline())

for line in f:
    res4.append(float(line))

f.close()

f = open(inputFileName5, "r")
res5 = []

step5 = int(f.readline())

for line in f:
    res5.append(float(line))

f.close()


plt.figure(figsize=(8,6))

plt.plot(range(0, len(res)*step, step), res, "r-", label="New")
plt.plot(range(0, len(res2)*step2, step2), res2, "g-", label="HllMinmod")
plt.plot(range(0, len(res3)*step3, step3), res3, "b-", label="HllcMinmod")
#plt.plot(range(0, len(res4)*step4, step4), res4, "k-", label="Minmod")
#plt.plot(range(0, len(res5)*step5, step5), res5, "y-", label="MinmodHll")
plt.yscale("log")
plt.ylabel("||rho||")
plt.xlabel("Iter")
plt.legend()

plt.tight_layout()
plt.savefig(outputFileName, dpi=200)

print("Byl vytvořen soubor ", outputFileName)

plt.close()