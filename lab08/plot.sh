set term png enhanced size 800,500 
set size ratio -1

set xl "x"
set yl "y"

# set contour
# set cntrparam levels 50
# unset surface
# set view map
# unset key

# set title "ψ(x,y), Q = -1000"
# set out "psi_Q_-1000.png"
# splot 'Q=-1000.000000.dat' u 1:2:3 w lines lt -1 palette lw 2 notitle 






set palette defined ( 0 '#000090',\
                      1 '#000fff',\
                      2 '#0090ff',\
                      3 '#0fffee',\
                      4 '#90ff70',\
                      5 '#ffee00',\
                      6 '#ff7000',\
                      7 '#ee0000',\
                      8 '#7f0000')
#u
set view map
unset contour
set surface

set title "v_x(x,y)"
set out "v_x_0.png"
splot 'vx_vy__D=0.000000.dat' u 1:2:3 w pm3d notitle

set title "v_y(x,y)"
set out "v_y_0.png"
splot 'vx_vy__D=0.000000.dat' u 1:2:4 w pm3d notitle



set title "v_x(x,y)"
set out "v_x_0.1.png"
splot 'vx_vy__D=0.100000.dat' u 1:2:3 w pm3d notitle

set title "v_y(x,y)"
set out "v_y_0.1.png"
splot 'vx_vy__D=0.100000.dat' u 1:2:4 w pm3d notitle




unset surface
set size ratio 0
set style line 1  lc rgb '#9999ff' lt 1 lw 2
set style line 2  lc rgb '#00ffcc' lt 1 lw 2
set style line 3  lc rgb '#ffcc00' lt 1 lw 2
set style line 4  lc rgb '#00ccff' lt 1 lw 2


set out "c.png"
set title "c( t_n)"
set xl "{/:Italic t_n}"
set yl "{/:Italic c}({/:Italic t_n})"
p "c__D=0.000000.dat" u 1:2 w lines t "D=0.0"   	ls 1,\
  "c__D=0.100000.dat" u 1:2 w lines t "D=0.1"   	ls 2,

set out "x_sr.png"
set title "x_{śr}(t_n)"
set xl "{/:Italic t_n}"
set yl "{/:Italic x_{śr}}({/:Italic t_n})"
p "x_sr__D=0.000000.dat" u 1:2 w lines t "D=0.0"   	ls 3,\
  "x_sr__D=0.100000.dat" u 1:2 w lines t "D=0.1"  	ls 4
  
  
 
set size ratio -1
set view map
unset contour
set surface
set cbr [0:]
set xl "x"
set yl "y"

set title "u(x,y)"
set out "u_0.0_1.png"
splot 'u__D=0.000000.dat' i 0 u 1:2:3 w pm3d notitle
set out "u_0.0_2.png"
splot 'u__D=0.000000.dat' i 4 u 1:2:3 w pm3d notitle
set out "u_0.0_3.png"
splot 'u__D=0.000000.dat' i 8 u 1:2:3 w pm3d notitle
set out "u_0.0_4.png"
splot 'u__D=0.000000.dat' i 12 u 1:2:3 w pm3d notitle
set out "u_0.0_5.png"
splot 'u__D=0.000000.dat' i 16 u 1:2:3 w pm3d notitle


set title "u(x,y)"
set out "u_0.1_1.png"
splot 'u__D=0.100000.dat' i 0 u 1:2:3 w pm3d notitle
set out "u_0.1_2.png"
splot 'u__D=0.100000.dat' i 4 u 1:2:3 w pm3d notitle
set out "u_0.1_3.png"
splot 'u__D=0.100000.dat' i 8 u 1:2:3 w pm3d notitle
set out "u_0.1_4.png"
splot 'u__D=0.100000.dat' i 12 u 1:2:3 w pm3d notitle
set out "u_0.1_5.png"
splot 'u__D=0.100000.dat' i 16 u 1:2:3 w pm3d notitle

# ------------------------------------------------------------------
# Rysowanie gifa
# ------------------------------------------------------------------
reset
set term gif size 800,300 animate delay 30

set output "Anim__u_D=0.0.gif"
n=19
set view map
set size ratio -1
set cbr [0:]
set title "u(x,y,Tk), D=0.0 (20 klatek)"
set xl "x"
set yl "y"

do for [k=0:n] {
	splot 'u__D=0.000000.dat' i k u 1:2:3 w pm3d notitle
} 


reset
set term gif size 800,300 animate delay 30
set output "Anim__u_D=0.1.gif"
n=19
set view map 
set size ratio -1
set cbr [0:]
set title "u(x,y,Tk), D=0.1 (20 klatek)"
set xl "x"
set yl "y"

do for [k=0:n] {
	splot 'u__D=0.100000.dat' i k u 1:2:3 w pm3d notitle
} 