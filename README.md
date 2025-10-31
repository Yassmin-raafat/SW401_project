# SW401 Phase 1 Project

## Project: Word Count (Phase 1)

### Description
This project implements a sequential and a shared-memory parallel version (OpenMP) of a word count program.

### Folder Structure
- `src/` : C++ source code
- `data/` : Sample input files
- `plots/` : Performance plots
- `report.pdf` : Project report

### How to Compile and Run
Sequential:
```bash
g++ -O3 src/wc_seq.cpp -o wc_seq
./wc_seq data/input.txt
