#!/bin/bash

OUTPUT="../plots/weak_scaling_results.txt"

echo "Weak Scaling Test" > $OUTPUT
echo "Problem size increases with number of processes" >> $OUTPUT
echo "-------------------------------------" >> $OUTPUT

# Generate random files
echo "Generating files..."
dd if=/dev/urandom of=../data/input_25MB.txt bs=1M count=25
dd if=/dev/urandom of=../data/input_50MB.txt bs=1M count=50
dd if=/dev/urandom of=../data/input_100MB.txt bs=1M count=100
dd if=/dev/urandom of=../data/input_200MB.txt bs=1M count=200

echo "Running Weak Scaling..."


# 1 rank → 25MB
echo -n "1 " >> $OUTPUT
START=$(date +%s.%N)
mpirun --allow-run-as-root -np 1 ../src/wc_mpi ../data/input_25MB.txt > /dev/null
END=$(date +%s.%N)
echo "$(echo $END - $START | bc)" >> $OUTPUT


# 2 ranks → 50MB
echo -n "2 " >> $OUTPUT
START=$(date +%s.%N)
mpirun --allow-run-as-root --oversubscribe -np 2 ../src/wc_mpi ../data/input_50MB.txt > /dev/null
END=$(date +%s.%N)
echo "$(echo $END - $START | bc)" >> $OUTPUT


# 4 ranks → 100MB
echo -n "4 " >> $OUTPUT
START=$(date +%s.%N)
mpirun --allow-run-as-root --oversubscribe -np 4 ../src/wc_mpi ../data/input_100MB.txt > /dev/null
END=$(date +%s.%N)
echo "$(echo $END - $START | bc)" >> $OUTPUT


# 8 ranks → 200MB
echo -n "8 " >> $OUTPUT
START=$(date +%s.%N)
mpirun --allow-run-as-root --oversubscribe -np 8 ../src/wc_mpi ../data/input_200MB.txt > /dev/null
END=$(date +%s.%N)
echo "$(echo $END - $START | bc)" >> $OUTPUT

echo "Done. Weak scaling results saved."
