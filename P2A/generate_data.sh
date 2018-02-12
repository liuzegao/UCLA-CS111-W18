echo > lab2_add.csv

# Run your program for ranges of iterations (100, 1000, 10000, 100000) values, capture the output, and note how many threads and iterations it takes to (fairly consistently) result in a failure (non-zero sum).
./lab2_add --threads=1  --iterations=100  >> lab2_add.csv
./lab2_add --threads=1  --iterations=1000 >> lab2_add.csv
./lab2_add --threads=1  --iterations=10000  >> lab2_add.csv
./lab2_add --threads=1  --iterations=100000 >> lab2_add.csv
./lab2_add --threads=2  --iterations=100  >> lab2_add.csv
./lab2_add --threads=2  --iterations=1000 >> lab2_add.csv
./lab2_add --threads=2  --iterations=10000  >> lab2_add.csv
./lab2_add --threads=2  --iterations=100000 >> lab2_add.csv
./lab2_add --threads=3  --iterations=100  >> lab2_add.csv
./lab2_add --threads=3  --iterations=1000 >> lab2_add.csv
./lab2_add --threads=3  --iterations=10000  >> lab2_add.csv
./lab2_add --threads=3  --iterations=100000 >> lab2_add.csv

# Re-run your tests, with yields, for ranges of threads (2,4,8,12) and iterations (10, 20, 40, 80, 100, 1000, 10000, 100000)...
./lab2_add --threads=2  --iterations=10 --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=20 --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=40 --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=80 --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=100  --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=1000 --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=10000  --yield  >> lab2_add.csv
./lab2_add --threads=2  --iterations=100000 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=10 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=20 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=40 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=80 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=100  --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=1000 --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=10000  --yield  >> lab2_add.csv
./lab2_add --threads=4  --iterations=100000 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=10 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=20 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=40 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=80 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=100  --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=1000 --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=10000  --yield  >> lab2_add.csv
./lab2_add --threads=8  --iterations=100000 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=20 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=40 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=80 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=100  --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000  --yield  >> lab2_add.csv
./lab2_add --threads=12 --iterations=100000 --yield  >> lab2_add.csv

# Compare the average execution time of the yield and non-yield versions a range threads (2, 8) and of iterations (100, 1000, 10000, 100000)
./lab2_add --threads=2  --iterations=100  --yield >> lab2_add.csv
./lab2_add --threads=2  --iterations=1000 --yield >> lab2_add.csv
./lab2_add --threads=2  --iterations=10000 --yield >> lab2_add.csv
./lab2_add --threads=2  --iterations=100000 --yield >> lab2_add.csv
./lab2_add --threads=8  --iterations=100  --yield >> lab2_add.csv
./lab2_add --threads=8  --iterations=1000 --yield >> lab2_add.csv
./lab2_add --threads=8  --iterations=10000 --yield >> lab2_add.csv
./lab2_add --threads=8  --iterations=100000 --yield >> lab2_add.csv

./lab2_add --threads=2  --iterations=100  >> lab2_add.csv
./lab2_add --threads=2  --iterations=1000 >> lab2_add.csv
./lab2_add --threads=2  --iterations=10000 >> lab2_add.csv
./lab2_add --threads=2  --iterations=100000 >> lab2_add.csv
./lab2_add --threads=8  --iterations=100  >> lab2_add.csv
./lab2_add --threads=8  --iterations=1000 >> lab2_add.csv
./lab2_add --threads=8  --iterations=10000 >> lab2_add.csv
./lab2_add --threads=8  --iterations=100000 >> lab2_add.csv

# Use your --yield options to confirm that, even for large numbers of threads (2, 4, 8, 12) and iterations (10,000 for mutexes and CAS, only 1,000 for spin locks)...
./lab2_add --threads=2 --iterations=10000  --yield  --sync=m  >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000  --yield  --sync=m  >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000  --yield  --sync=m  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield  --sync=m  >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000  --yield  --sync=c  >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000  --yield  --sync=c  >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000  --yield  --sync=c  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield  --sync=c  >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000  --yield  --sync=s  >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000  --yield  --sync=s  >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000  --yield  --sync=s  >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --yield  --sync=s  >> lab2_add.csv

# Using a large enough number of iterations (e.g. 10,000) to overcome the issues raised in the question 2.1.3, test all four (no yield) versions (unprotected, mutex, spin-lock, compare-and-swap) for a range of number of threads (1,2,4,8,12)...
./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=m  >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=m  >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=m  >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=m  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=m  >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=s  >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=s  >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=s  >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=s  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=s  >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=c  >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=c  >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=c  >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=c  >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=c  >> lab2_add.csv

# Additional test cases
./lab2_add --threads=1  --iterations=100  --yield  >> lab2_add.csv
./lab2_add --threads=1  --iterations=1000 --yield  >> lab2_add.csv
./lab2_add --threads=1  --iterations=10000  --yield  >> lab2_add.csv
./lab2_add --threads=1  --iterations=100000 --yield  >> lab2_add.csv

./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv





echo > lab2_list.csv
# Run your program with a single thread, and increasing numbers of iterations (10, 100, 1000, 10000, 20000)
./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

# Run your program and see how many parallel threads (2,4,8,12) and iterations (1, 10,100,1000) it takes to fairly consistently demonstrate a problem. 
./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

# Then run it again using various combinations of yield options and see how many threads (2,4,8,12) and iterations (1, 2,4,8,16,32) it takes to fairly consistently demonstrate the problem
./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 >> lab2_list.csv

# conflicts between inserts (--yield=i)
./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv

# conflicts between deletes (--yield=d)
./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv

# conflicts between inserts and lookups (--yield=il)
./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv

# conflicts between deletes and lookups (--yield=dl)
./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv

# Using your --yield options, demonstrate that either of these protections seems to eliminate all of the problems, even for moderate numbers of threads (12) and iterations (32)
./lab2_list --threads=12 --iterations=32 --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --sync=s >> lab2_list.csv

./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv

# Choose an appropriate number of iterations (e.g. 1000) to overcome start-up costs and rerun your program without the yields for a range of # threads (1, 2, 4, 8, 12, 16, 24)
./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 >> lab2_list.csv


