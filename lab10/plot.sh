set term png enhanced size 800,500 


set style line 1  lc rgb '#ffcc00' lt 1 lw 2
set style line 2  lc rgb '#00ccff' lt 1 lw 2
set style line 3  lc rgb '#00ffcc' lt 1 lw 2


set out "E1.png"
set title "{/:Italic E}({/:Italic t})"
set key right bottom
set xl "{/:Italic t}"
set yl "{/:Italic E}"
plot "E_alfa_0.000000_beta_0.000000.dat" u 1:2 w lines title "α=0.0, β=0.0" ls 1, \
	  "E_alfa_0.000000_beta_0.100000.dat" u 1:2 w lines title "α=0.0, β=0.1" ls 2, \
	  "E_alfa_0.000000_beta_1.000000.dat" u 1:2 w lines title "α=0.0, β=1.0" ls 3

set out "E2.png"
plot "E_alfa_1.000000_beta_1.000000.dat" u 1:2 w lines title "α=1.0, β=1.0" ls 3



set term png size 1000,400 
set view map
unset contour
set surface
set xl "{/:Italic t}"
set yl "{/:Italic x}"


set out "u1.png"
set title "α=0.0, β=0.0"
splot 'u_alfa_0.000000_beta_0.000000.dat' u 1:2:3 w pm3d notitle
set out "u2.png"
set title "α=0.0, β=0.1"
splot 'u_alfa_0.000000_beta_0.100000.dat' u 1:2:3 w pm3d notitle
set out "u3.png"
set title "α=0.0, β=1.0"
set cbrange [-0.2:1.2]
splot 'u_alfa_0.000000_beta_1.000000.dat' u 1:2:3 w pm3d notitle
set out "u4.png"
set cbrange [-0.05:0.04]
set title "α=1.0, β=1.0"
splot 'u_alfa_1.000000_beta_1.000000.dat' u 1:2:3 w pm3d notitle


