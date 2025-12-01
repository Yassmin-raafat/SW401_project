#!/bin/bash

FILE="../data/input.txt"
OUTPUT="../plots/strong_scaling_results.txt"

echo "Strong Scaling Test" > $OUTPUT
echo "Problem size = fixed (input.txt)" >> $OUTPUT
echo "---------------------------------" >> $OUTPUT

for p in 1 2 4 8 16
do
    echo "Running with $p processes..." | tee -a $OUTPUT

    START=$(date +%s.%N)

    mpirun --allow-run-as-root --oversubscribe -np $p ../src/wc_mpi $FILE > /dev/null

    END=$(date +%s.%N)

    RUNTIME=$(echo "$END - $START" | bc)

    echo "$p $RUNTIME" >> $OUTPUT
done

echo "Done. Results saved in strong_scaling_results.txt"
