import matplotlib.pyplot as plt

# -------------------------------
# Load Strong Scaling Data
# -------------------------------
strong_file = "../plots/strong_scaling_results.txt"

procs_s = []
times_s = []

with open(strong_file, "r") as f:
    lines = f.readlines()

# Skip first 3 header lines
for line in lines[3:]:
    parts = line.strip().split()
    if len(parts) == 2 and parts[0].isdigit():
        procs_s.append(int(parts[0]))
        times_s.append(float(parts[1]))

# Compute speedup & efficiency
T1 = times_s[0]
speedup = [T1 / t for t in times_s]
efficiency = [speedup[i] / procs_s[i] for i in range(len(procs_s))]

# -------------------------------
# Strong Scaling Plots
# -------------------------------
# Runtime
plt.figure()
plt.plot(procs_s, times_s, marker='o')
plt.xlabel("Processes")
plt.ylabel("Runtime (s)")
plt.title("Strong Scaling: Runtime vs Processes")
plt.grid(True)
plt.savefig("../plots/strong_runtime.png")
plt.close()

# Speedup
plt.figure()
plt.plot(procs_s, speedup, marker='o')
plt.xlabel("Processes")
plt.ylabel("Speedup")
plt.title("Strong Scaling: Speedup vs Processes")
plt.grid(True)
plt.savefig("../plots/strong_speedup.png")
plt.close()

# Efficiency
plt.figure()
plt.plot(procs_s, efficiency, marker='o')
plt.xlabel("Processes")
plt.ylabel("Efficiency")
plt.title("Strong Scaling: Efficiency vs Processes")
plt.grid(True)
plt.savefig("../plots/strong_efficiency.png")
plt.close()


# -------------------------------
# Load Weak Scaling Data
# -------------------------------
weak_file = "../plots/weak_scaling_results.txt"

procs_w = []
times_w = []

with open(weak_file, "r") as f:
    lines = f.readlines()

# Skip first 3 header lines
for line in lines[3:]:
    parts = line.strip().split()
    if len(parts) == 2 and parts[0].isdigit():
        procs_w.append(int(parts[0]))
        times_w.append(float(parts[1]))

# Weak Scaling Plot
plt.figure()
plt.plot(procs_w, times_w, marker='o')
plt.xlabel("Processes")
plt.ylabel("Runtime (s)")
plt.title("Weak Scaling: Runtime vs Processes")
plt.grid(True)
plt.savefig("../plots/weak_runtime.png")
plt.close()

print("All plots generated successfully in ../plots/")
