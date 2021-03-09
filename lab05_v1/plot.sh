set term png size 800,800


set style line 1  lc rgb '#9999ff' lt 1 lw 3
set style line 2  lc rgb '#00ffcc' lt 1 lw 3 
set style line 3  lc rgb '#ffcc00' lt 1 lw 3
set style line 4  lc rgb '#00ccff' lt 1 lw 3 
set style line 5  lc rgb '#ff0055' lt 1 lw 3 


set out "w1.png"
set title "{/:Italic S}({/:Italic it})"
set xl "{/:Italic it}"
set yl "{/:Italic S}"
#set logscale x

p "S_k=16.dat" u 1:2 w lines t "{/:Italic k} = 16" ls 1,\
  "S_k=8.dat" u 1:2 w lines t "{/:Italic k} = 8"   ls 2,\
  "S_k=4.dat" u 1:2 w lines t "{/:Italic k} = 4"   ls 3,\
  "S_k=2.dat" u 1:2 w lines t "{/:Italic k} = 2"   ls 4,\
  "S_k=1.dat" u 1:2 w lines t "{/:Italic k} = 1"   ls 5

#unset logscale x



set pm3d map
set xl "{/:Italic x}"
set yl "{/:Italic y}"


set xrange [0:128]
set yrange [0:128]

set out "w2.png"
set title "{/:Italic k} = 16"
splot "V_k=16.dat" u 1:2:3 
set out "w3.png"
set title "{/:Italic k} = 8"
splot "V_k=8.dat" u 1:2:3 
set out "w4.png"
set title "{/:Italic k} = 4"
splot "V_k=4.dat" u 1:2:3 
set out "w5.png"
set title "{/:Italic k} = 2"
splot "V_k=2.dat" u 1:2:3 
set out "w6.png"
set title "{/:Italic k} = 1"
splot "V_k=1.dat" u 1:2:3 

