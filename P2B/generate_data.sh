echo > lab2b_list.csv

# Mutex synchronized list operations, 1,000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --lists=1 --sync=m >> lab2b_list.csv

# Spin-lock synchronized list operations, 1,000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --lists=1 --sync=s >> lab2b_list.csv