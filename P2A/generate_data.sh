echo > lab2_add.csv

# Run your program for ranges of iterations (100, 1000, 10000, 100000) values, capture the output, and note how many threads and iterations it takes to (fairly consistently) result in a failure (non-zero sum).
./lab2_add      --threads=1     --iterations=100                 >> lab2_add.csv
./lab2_add      --threads=1     --iterations=1000                >> lab2_add.csv
./lab2_add      --threads=1     --iterations=10000               >> lab2_add.csv
./lab2_add      --threads=1     --iterations=100000              >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100                 >> lab2_add.csv
./lab2_add      --threads=2     --iterations=1000                >> lab2_add.csv
./lab2_add      --threads=2     --iterations=10000               >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100000              >> lab2_add.csv
./lab2_add      --threads=3     --iterations=100                 >> lab2_add.csv
./lab2_add      --threads=3     --iterations=1000                >> lab2_add.csv
./lab2_add      --threads=3     --iterations=10000               >> lab2_add.csv
./lab2_add      --threads=3     --iterations=100000              >> lab2_add.csv

# Re-run your tests, with yields, for ranges of threads (2,4,8,12) and iterations (10, 20, 40, 80, 100, 1000, 10000, 100000)...
./lab2_add      --threads=2     --iterations=10        --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=20        --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=40        --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=80        --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100       --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=1000      --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=10000     --yield     >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100000    --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=10        --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=20        --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=40        --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=80        --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=100       --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=1000      --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=10000     --yield     >> lab2_add.csv
./lab2_add      --threads=4     --iterations=100000    --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=10        --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=20        --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=40        --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=80        --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100       --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=1000      --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=10000     --yield     >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100000    --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=10        --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=20        --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=40        --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=80        --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=100       --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=1000      --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=10000     --yield     >> lab2_add.csv
./lab2_add      --threads=12    --iterations=100000    --yield     >> lab2_add.csv

# Compare the average execution time of the yield and non-yield versions a range threads (2, 8) and of iterations (100, 1000, 10000, 100000)
./lab2_add      --threads=2     --iterations=100       --yield >> lab2_add.csv
./lab2_add      --threads=2     --iterations=1000      --yield >> lab2_add.csv
./lab2_add      --threads=2     --iterations=10000      --yield >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100000      --yield >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100       --yield >> lab2_add.csv
./lab2_add      --threads=8     --iterations=1000      --yield >> lab2_add.csv
./lab2_add      --threads=8     --iterations=10000      --yield >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100000      --yield >> lab2_add.csv

./lab2_add      --threads=2     --iterations=100       >> lab2_add.csv
./lab2_add      --threads=2     --iterations=1000      >> lab2_add.csv
./lab2_add      --threads=2     --iterations=10000      >> lab2_add.csv
./lab2_add      --threads=2     --iterations=100000      >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100       >> lab2_add.csv
./lab2_add      --threads=8     --iterations=1000      >> lab2_add.csv
./lab2_add      --threads=8     --iterations=10000      >> lab2_add.csv
./lab2_add      --threads=8     --iterations=100000      >> lab2_add.csv

# Use your --yield options to confirm that, even for large numbers of threads (2, 4, 8, 12) and iterations (10,000 for mutexes and CAS, only 1,000 for spin locks)...
./lab2_add --threads=2 --iterations=10000       --yield     --sync=m     >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000       --yield     --sync=m     >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000       --yield     --sync=m     >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000      --yield     --sync=m     >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000       --yield     --sync=c     >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000       --yield     --sync=c     >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000       --yield     --sync=c     >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000      --yield     --sync=c     >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000       --yield     --sync=s     >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000       --yield     --sync=s     >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000       --yield     --sync=s     >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000      --yield     --sync=s     >> lab2_add.csv

# Using a large enough number of iterations (e.g. 10,000) to overcome the issues raised in the question 2.1.3, test all four (no yield) versions (unprotected, mutex, spin-lock, compare-and-swap) for a range of number of threads (1,2,4,8,12)...
./lab2_add --threads=1 --iterations=10000              >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000              >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000              >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000              >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000              >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000            --sync=m     >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000            --sync=m     >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000            --sync=m     >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000            --sync=m     >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000            --sync=m     >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000            --sync=s     >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000            --sync=s     >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000            --sync=s     >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000            --sync=s     >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000            --sync=s     >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000            --sync=c     >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000            --sync=c     >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000            --sync=c     >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000            --sync=c     >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000            --sync=c     >> lab2_add.csv

# Additional test cases
./lab2_add      --threads=1     --iterations=100       --yield     >> lab2_add.csv
./lab2_add      --threads=1     --iterations=1000      --yield     >> lab2_add.csv
./lab2_add      --threads=1     --iterations=10000     --yield     >> lab2_add.csv
./lab2_add      --threads=1     --iterations=100000    --yield     >> lab2_add.csv

./lab2_add --threads=1 --iterations=100000              >> lab2_add.csv

