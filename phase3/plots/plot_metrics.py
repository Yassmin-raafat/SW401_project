import pandas as pd
import matplotlib.pyplot as plt

CSV = "phase3/logs/client_metrics.csv"

# Read metrics
df = pd.read_csv(CSV)

# Normalize time (start from 0)
t0 = df["timestamp"].iloc[0]
df["time_s"] = df["timestamp"] - t0

# =========================
# Latency vs Time
# =========================
plt.figure(figsize=(8, 4))
plt.plot(df["time_s"], df["latency_ms"], label="Latency")

# Failure annotation 
plt.axvline(x=30, color="red", linestyle="--", label="Service Crash")

plt.xlabel("Time (s)")
plt.ylabel("Latency (ms)")
plt.title("Latency vs Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("phase3/plots/latency_vs_time.png", dpi=200)
plt.close()

# =========================
# Throughput vs Time
# =========================
plt.figure(figsize=(8, 4))
plt.plot(df["time_s"], df["throughput_rps"], label="Throughput")

# Failure annotation
plt.axvline(x=30, color="red", linestyle="--", label="Service Crash")

plt.xlabel("Time (s)")
plt.ylabel("Throughput (req/s)")
plt.title("Throughput vs Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("phase3/plots/throughput_vs_time.png", dpi=200)
plt.close()

print("Saved plots:")
print(" - phase3/plots/latency_vs_time.png")
print(" - phase3/plots/throughput_vs_time.png")
