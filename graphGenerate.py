import matplotlib.pyplot as plt

with open("graphUnoptimizedMmult.txt", "r") as f:
    lines = f.readlines()

non_vectorized = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:500]]
vectorized = [[int(x.split()[0]), float(x.split()[1])] for x in lines[501:]]

with open("graphSIMD.txt", "r") as f:
    lines = f.readlines()

non_vectorized2 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:500]]
vectorized2 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[501:]]

with open("graphOMP.txt", "r") as f:
    lines = f.readlines()

non_vectorized3 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:500]]
vectorized3 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[501:]]

with open("graphMPIOMP.txt", "r") as f:
    lines = f.readlines()

mpiomp = [[i+1, float(lines[i].strip())] for i in range(len(lines))]

plt.plot([x[0] for x in non_vectorized], [x[1] for x in non_vectorized], label="Non-Vectorized (Original)")
plt.plot([x[0] for x in vectorized], [x[1] for x in vectorized], label="Vectorized (Original)")
plt.plot([x[0] for x in non_vectorized2], [x[1] for x in non_vectorized2], label="Non-Vectorized (SIMD)")
plt.plot([x[0] for x in vectorized2], [x[1] for x in vectorized2], label="Vectorized (SIMD)")
plt.plot([x[0] for x in non_vectorized3], [x[1] for x in non_vectorized3], label="Non-Vectorized (OMP)")
plt.plot([x[0] for x in vectorized3], [x[1] for x in vectorized3], label="Vectorized (OMP)")
plt.plot([x[0] for x in mpiomp], [x[1] for x in mpiomp], label="MPI+OMP")

plt.xlabel("Matrix Size")
plt.ylabel("Execution Time (seconds)")
plt.title("Matrix Multiplication Performance")

plt.legend()

plt.show()
