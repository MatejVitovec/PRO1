import sys
from matplotlib import pyplot as plt

inputFileName = "PostProcess/residue50GodunovHllc.txt"
inputFileName2 = "PostProcess/residue50MinmodHllc.txt"
inputFileName3 = "PostProcess/residue50VanAlbadaHllc.txt"
inputFileName4 = "PostProcess/residue50SuperbeeHllc.txt"
inputFileName5 = "PostProcess/residue50MCHllc.txt"

inputFileName = "PostProcess/residue50GodunovHll.txt"
inputFileName2 = "PostProcess/residue50GodunovHllc.txt"
inputFileName3 = "PostProcess/residue50MinmodHll.txt"
inputFileName4 = "PostProcess/residue50MinmodHllc.txt"

inputFileName = "PostProcess/residue50GodunovHllc.txt"
inputFileName2 = "PostProcess/residue200GodunovHllc.txt"
inputFileName3 = "PostProcess/residue500GodunovHllc.txt"
outputFileName = "PostProcess/residue500Hllc.png"


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

plt.plot(range(0, step), res, "r-", label="n = 50")
plt.plot(range(0, step2), res2, "g-", label="n = 200")
plt.plot(range(0, step3), res3, "b-", label="n = 500")
#plt.plot(range(0, step4), res4, "k-", label="Minmod HLLC")
#plt.plot(range(0, step5), res5, "y-", label="MC")

plt.yscale("log")
plt.ylabel("||rho||")
plt.xlabel("Iter")
plt.legend()

plt.tight_layout()
plt.savefig(outputFileName, dpi=200)

print("Byl vytvořen soubor ", outputFileName)

plt.close()