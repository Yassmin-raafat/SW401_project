import time
import os
import re
from concurrent import futures
import grpc

import sys
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "proto"))
import wc_pb2
import wc_pb2_grpc

WORD_RE = re.compile(r"[A-Za-z0-9]+")

def count_words(text: str) -> int:
   
    return len(WORD_RE.findall(text))

class WordCountService(wc_pb2_grpc.WordCountServiceServicer):
    def __init__(self, replica_id, log_file):
        self.replica_id = replica_id
        self.log_file = log_file

    def CountWords(self, request, context):
        start = time.time()
        words = count_words(request.text)
        end = time.time()

        latency_ms = (end - start) * 1000.0

        with open(self.log_file, "a") as f:
            f.write(f"{time.time():.6f},{request.request_id},{self.replica_id},{latency_ms:.3f},{words}\n")

        return wc_pb2.CountReply(
            word_count=words,
            server_id=self.replica_id
        )

def serve():
    replica_id = os.environ.get("REPLICA_ID", "replica-1")
    port = int(os.environ.get("PORT", "50051"))
    log_file = os.environ.get("LOG_FILE", f"phase3/logs/{replica_id}.log")

    os.makedirs(os.path.dirname(log_file), exist_ok=True)

    server = grpc.server(futures.ThreadPoolExecutor(max_workers=8))
    wc_pb2_grpc.add_WordCountServiceServicer_to_server(
        WordCountService(replica_id, log_file), server
    )

    server.add_insecure_port(f"[::]:{port}")
    server.start()
    print(f"[{replica_id}] running on port {port}")
    server.wait_for_termination()

if __name__ == "__main__":
    serve()
