# GRAFICA CUATRO GRAFICAS EN UNA VENTANA
# l@rry
#set term corel
set output  "multi_graph.eps"

set autoscale
set multiplot layout 2,2 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "7 cycles"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/7_cycles.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "15 cycles"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/15_cycles.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 3 ----------------
set title "25 cycles"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/25_cycles.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 5 ----------------
set title "3 tones"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/3_tones.dat"
unset xlabel
unset ylabel
unset title


#pause(3)
#unset multiplot

#