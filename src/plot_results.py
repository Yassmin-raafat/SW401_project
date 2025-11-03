import matplotlib.pyplot as plt


threads = [1, 2, 4, 8]
runtime = [0.629682, 0.397797, 0.297168, 0.295796]  

# Compute speedup and efficiency 
T1 = runtime[0]
speedup = [T1 / t for t in runtime]
efficiency = [s / p for s, p in zip(speedup, threads)]

#  Plot 1: Speedup vs Threads 
plt.figure(figsize=(6,4))
plt.plot(threads, speedup, marker='o', color='b')
plt.title("Speedup vs Threads")
plt.xlabel("Threads")
plt.ylabel("Speedup (S = T₁ / Tₚ)")
plt.grid(True)
plt.savefig("../plots/speedup_vs_threads.png", dpi=300)

#  Plot 2: Efficiency vs Threads
plt.figure(figsize=(6,4))
plt.plot(threads, efficiency, marker='o', color='g')
plt.title("Efficiency vs Threads")
plt.xlabel("Threads")
plt.ylabel("Efficiency (E = S / p)")
plt.grid(True)
plt.savefig("../plots/efficiency_vs_threads.png", dpi=300)

print(" Plots saved in ../plots/")
