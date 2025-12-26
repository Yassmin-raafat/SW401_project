from pyspark.sql import SparkSession
import grpc
import time
import os
import sys

sys.path.append("phase3/proto")
import wc_pb2
import wc_pb2_grpc

SERVERS = ["localhost:50051", "localhost:50052"]


METRICS_CSV = "phase3/logs/client_metrics.csv"
os.makedirs(os.path.dirname(METRICS_CSV), exist_ok=True)
if not os.path.exists(METRICS_CSV):
    with open(METRICS_CSV, "w") as f:
        f.write("timestamp,latency_ms,throughput_rps,server\n")

_last_ts = time.time()
_req_count = 0

def log_metrics(latency_ms, server):
    global _last_ts, _req_count
    _req_count += 1
    now = time.time()
    dt = now - _last_ts
    if dt <= 0:
        dt = 1e-9
    throughput = _req_count / dt

    with open(METRICS_CSV, "a") as f:
        f.write(f"{now:.6f},{latency_ms:.3f},{throughput:.3f},{server}\n")

def call_grpc(batch_df, batch_id):
   
    if batch_df.rdd.isEmpty():
        return

    text = "hello spark streaming grpc test " * 5

    for host in SERVERS:
        channel = None
        try:
            t0 = time.time()

            channel = grpc.insecure_channel(host)  
            stub = wc_pb2_grpc.WordCountServiceStub(channel)

            resp = stub.CountWords(
                wc_pb2.CountRequest(text=text, request_id=str(batch_id)),
                timeout=2  
            )

            latency_ms = (time.time() - t0) * 1000.0
            print(f"[Spark batch {batch_id}] {host} → {resp.word_count} (lat={latency_ms:.1f}ms)")

            log_metrics(latency_ms, host)
            return

        except Exception as e:
            print(f"[Spark batch {batch_id}] {host} failed → retry next | {type(e).__name__}: {e}")

        finally:
            if channel is not None:
                channel.close()

    print(f"[Spark batch {batch_id}] all replicas failed (will continue, no crash)")

spark = SparkSession.builder \
    .appName("WordCountSparkStreaming") \
    .getOrCreate()

spark.sparkContext.setLogLevel("ERROR")

df = spark.readStream.format("rate").option("rowsPerSecond", 2).load()

query = df.writeStream.foreachBatch(call_grpc).start()
query.awaitTermination()
