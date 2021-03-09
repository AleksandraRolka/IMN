set term png size 1600,1000


set style line 1  lc rgb '#9999ff' lt 1 lw 3
set style line 2  lc rgb '#00ffcc' lt 1 lw 3 
set style line 3  lc rgb '#ffcc00' lt 1 lw 3
set style line 4  lc rgb '#00ccff' lt 1 lw 3 


set out "w1.png"
set title "Relaksacja globalna {/:Italic S}({/:Italic it})"
set xl "{/:Italic it}"
set yl "{/:Italic S}"
set logscale x
p "global_relaxation_S_0.600000.dat" u 1:2 w lines t "{/:Italic ω_G} = 0.6"   ls 2,\
  "global_relaxation_S_1.000000.dat" u 1:2 w lines t "{/:Italic ω_G} = 1.0"   ls 1
  
set out "w2.png"
set title "Relaksacja lokalna {/:Italic S}({/:Italic it})"
set xl "{/:Italic it}"
set yl "{/:Italic S}"
p "local_relaxation_S_1.000000.dat" u 1:2 w lines t "{/:Italic ω_L} = 1.0"   ls 1,\
  "local_relaxation_S_1.400000.dat" u 1:2 w lines t "{/:Italic ω_L} = 1.4"   ls 3,\
  "local_relaxation_S_1.800000.dat" u 1:2 w lines t "{/:Italic ω_L} = 1.8"   ls 2,\
  "local_relaxation_S_1.900000.dat" u 1:2 w lines t "{/:Italic ω_L} = 1.9"   ls 4


unset logscale x


set pm3d map
set xl "{/:Italic x}"
set yl "{/:Italic y}"


set xrange [0:150]
set yrange [0:100]
set out "w3.png"
set title "Relaksacja globalna - {/:Italic V} ({/:Italic x},{/:Italic y})\ndla {/:Italic ω_G} = 0.6"
splot "global_relaxation_V_0.600000.dat" u 1:2:3 
set out "w4.png"
set title "Relaksacja globalna - {/:Italic V} ({/:Italic x},{/:Italic y})\ndla {/:Italic ω_G} = 1.0"
splot "global_relaxation_V_1.000000.dat" u 1:2:3 


set xrange [0:15]
set yrange [0:10]
set out "w5.png"
set title "Relaksacja globalna\n-mapa błędu rozwiązania δ dla {/:Italic ω_G} = 0.6"
splot "global_relaxation_err_0.600000.dat" u 1:2:3 
set out "w6.png"
set title "Relaksacja globalna\n-mapa błędu rozwiązania δ dla {/:Italic ω_G} = 1.0"
splot "global_relaxation_err_1.000000.dat" u 1:2:3 

