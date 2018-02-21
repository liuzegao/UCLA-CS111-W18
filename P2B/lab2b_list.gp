# general plot parameters
set terminal png
set datafile separator ","


# lab2b_1.png
set title "lab2b-1: Throughput vs. Number of threads"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Throughput (op/s)"
set logscale y 10
set output 'lab2b_1.png'

# grep
plot \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'Mutex Method' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-s,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'Spin-lock Method' with linespoints lc rgb 'green'


# lab2b_2.png
set title "lab2b-2: Time vs Number of Threads for Mutex"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Time (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top

# grep
plot \
    "< grep -E 'list-none-m,[0-9],1000,1,|list-none-m,16,1000,1,|list-none-m,24,1000,1,' lab2b_list.csv" using ($2):($8) \
        title 'Wait-for-lock Time' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-m,[0-9],1000,1,|list-none-m,16,1000,1,|list-none-m,24,1000,1,' lab2b_list.csv" using ($2):($7) \
        title 'Time per operation' with linespoints lc rgb 'green'
     

# lab2b_3.png
set title "lab2b-3: Successful runs of varying Synchronization Methods"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Iterations"
set logscale y 10
set output 'lab2b_3.png'
set key left top

# grep
plot \
    "< grep -E 'list-id-none,[0-9]+,[0-9]+,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "red" title "None", \
    "< grep -E 'list-id-m,[0-9]+,[0-9]+,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "green" title "Mutex", \
    "< grep -E 'list-id-s,[0-9]+,[0-9]+,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "blue" title "Spin-Lock"


# lab2b_4.png
set title "lab2b-4: Mutex Throughput vs. Number of threads"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Throughput (op/s)"
set logscale y 10
set output 'lab2b_4.png'

# grep
plot \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '1 Sublist' with linespoints lc rgb 'purple', \
    "< grep -E 'list-none-m,[0-9]+,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '4 Sublist' with linespoints lc rgb 'green', \
    "< grep -E 'list-none-m,[0-9]+,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '8 Sublist' with linespoints lc rgb 'blue', \
    "< grep -E 'list-none-m,[0-9]+,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '16 Sublist' with linespoints lc rgb 'gold'


# lab2b_5.png
set title "lab2b-5: Spinlock Throughput vs. Number of threads"
set xlabel "Threads"
set logscale x 2
set xrange[0.75:]
set ylabel "Throughput (op/s)"
set logscale y 10
set output 'lab2b_5.png'

# grep
plot \
    "< grep -E 'list-none-s,[0-9]+,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '1 Sublist' with linespoints lc rgb 'purple', \
    "< grep -E 'list-none-s,[0-9]+,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '4 Sublist' with linespoints lc rgb 'green', \
    "< grep -E 'list-none-s,[0-9]+,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '8 Sublist' with linespoints lc rgb 'blue', \
    "< grep -E 'list-none-s,[0-9]+,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title '16 Sublist' with linespoints lc rgb 'gold'

