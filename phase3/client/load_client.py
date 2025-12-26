import os
import time
import uuid
import random
import csv
import grpc
from collections import deque

import sys
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "proto"))
import wc_pb2
import wc_pb2_grpc

SERVERS = [
    ("localhost", 50051),
    ("localhost", 50052),
]

DIAL_TIMEOUT = 0.5
RPC_TIMEOUT = 0.7

def random_text():
    words = [
        "parallel", "mpi", "openmp", "grpc", "fault",
        "tolerance", "scaling", "distributed", "system", "count"
    ]
    return " ".join(random.choice(words) for _ in range(random.randint(50, 200)))

def main():
    duration = 60          # seconds
    rate = 15              # requests per second
    out_csv = "phase3/logs/client_metrics.csv"

    os.makedirs("phase3/logs", exist_ok=True)

    period = 1.0 / rate
    end_time = time.time() + duration

    sent_times = deque()
    idx = 0

    with open(out_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "timestamp", "request_id", "server",
            "success", "latency_ms", "throughput_rps"
        ])

        while time.time() < end_time:
            t_send = time.time()
            request_id = str(uuid.uuid4())
            text = random_text()

            success = 0
            latency_ms = ""
            server_used = ""

            for _ in range(len(SERVERS)):
                host, port = SERVERS[idx % len(SERVERS)]
                idx += 1
                try:
                    channel = grpc.insecure_channel(f"{host}:{port}")
                    grpc.channel_ready_future(channel).result(timeout=DIAL_TIMEOUT)

                    stub = wc_pb2_grpc.WordCountServiceStub(channel)
                    t0 = time.time()
                    resp = stub.CountWords(
                        wc_pb2.CountRequest(text=text, request_id=request_id),
                        timeout=RPC_TIMEOUT
                    )
                    t1 = time.time()

                    success = 1
                    latency_ms = (t1 - t0) * 1000
                    server_used = f"{host}:{port} ({resp.server_id})"
                    break
                except Exception:
                    continue

            sent_times.append(t_send)
            while sent_times and (t_send - sent_times[0]) > 1.0:
                sent_times.popleft()
            throughput = len(sent_times)

            writer.writerow([
                f"{t_send:.6f}", request_id, server_used,
                success, latency_ms, throughput
            ])
            f.flush()

            sleep_time = period - (time.time() - t_send)
            if sleep_time > 0:
                time.sleep(sleep_time)

    print(f"Client finished. Metrics saved to {out_csv}")

if __name__ == "__main__":
    main()
