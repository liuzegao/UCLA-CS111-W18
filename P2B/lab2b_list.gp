# general plot parameters
set terminal png
set datafile separator ","

# how many threads/iterations we can run without failure (w/o yielding)
set title "List-1: Throughput vs. Number of threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Throughput (op/ns)"
set logscale y 10
set output 'lab2_list-1.png'

# grep out only successful (sum=0)  unsynchronized runs
plot \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'Mutex Method' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-s,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'Spin-lock Method' with linespoints lc rgb 'blue'

