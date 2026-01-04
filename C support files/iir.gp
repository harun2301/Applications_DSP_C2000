# GRAFICA DOS GRAFICAS EN UNA VENTANA
# l@rry
#set term corel
set output  "multi_graph.eps"

set autoscale
set multiplot layout 2,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "3 tones"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/3_tones.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "filtered"
unset label
set xlabel   "n"
set ylabel   "y(n)"
plot  "data_files/filtered.dat"
unset xlabel
unset ylabel
unset title


#pause(3)
#unset multiplot

#