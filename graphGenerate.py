import matplotlib.pyplot as plt

with open("graphUnpotimizedMmult.txt", "r") as f:
    lines = f.readlines()

non_vectorized = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:48]]
vectorized = [[int(x.split()[0]), float(x.split()[1])] for x in lines[48:]]

with open("graphSIMD.txt", "r") as f:
    lines = f.readlines()

non_vectorized2 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:48]]
vectorized2 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[48:]]

with open("graphOMP.txt", "r") as f:
    lines = f.readlines()

non_vectorized3 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[:48]]
vectorized3 = [[int(x.split()[0]), float(x.split()[1])] for x in lines[48:]]


plt.plot([x[0] for x in non_vectorized], [x[1] for x in non_vectorized], label="Non-Vectorized (Original)")
plt.plot([x[0] for x in vectorized], [x[1] for x in vectorized], label="Vectorized (Original)")
plt.plot([x[0] for x in non_vectorized2], [x[1] for x in non_vectorized2], label="Non-Vectorized (SIMD)")
plt.plot([x[0] for x in vectorized2], [x[1] for x in vectorized2], label="Vectorized (SIMD)")
plt.plot([x[0] for x in non_vectorized3], [x[1] for x in non_vectorized3], label="Non-Vectorized (OMP)")
plt.plot([x[0] for x in vectorized3], [x[1] for x in vectorized3], label="Vectorized (OMP)")


plt.xlabel("Matrix Size")
plt.ylabel("Execution Time (seconds)")
plt.title("Matrix Multiplication Performance")

plt.legend()

plt.show()