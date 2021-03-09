set term png #size 1000,1000

set style line 1  lc rgb '#ff66ff' lt 1 lw 2
set style line 2  lc rgb '#0099ff' lt 1 lw 2 
set style line 3  lc rgb '#ffd633' lt 1 lw 2 
set style line 4  lc rgb '#00ff99' lt 1 lw 2


set xl "t"
set yl "y(t)"

set out "Metoda_Eulera.png"
set title "Metoda Eulera"
p "explicit_Euler_method_0.010000.dat" u 1:2 w lines t "dt = 0.01"   ls 1,\
  "explicit_Euler_method_0.100000.dat" u 1:2 w lines t "dt = 0.1"    ls 2,\
  "explicit_Euler_method_1.000000.dat" u 1:2 w lines t "dt = 1.0"    ls 3,\
  "explicit_Euler_method_0.010000.dat" u 1:3 w lines t "analityczne" ls 4
 
set out "Metoda_RK2.png"
set title "Metoda RK2"
p "explicit_RK2_method_0.010000.dat" u 1:2 w lines t "dt = 0.01"   ls 1,\
  "explicit_RK2_method_0.100000.dat" u 1:2 w lines t "dt = 0.1"    ls 2,\
  "explicit_RK2_method_1.000000.dat" u 1:2 w lines t "dt = 1.0"    ls 3,\
  "explicit_RK2_method_0.010000.dat" u 1:3 w lines t "analityczne" ls 4
  
set out "Metoda_RK4.png"
set title "Metoda RK4"
p "explicit_RK4_method_0.010000.dat" u 1:2 w lines t "dt = 0.01"   ls 1,\
  "explicit_RK4_method_0.100000.dat" u 1:2 w lines t "dt = 0.1"    ls 2,\
  "explicit_RK4_method_1.000000.dat" u 1:2 w lines t "dt = 1.0"    ls 3,\
  "explicit_RK4_method_0.010000.dat" u 1:3 w lines t "analityczne" ls 4
  
  
  
#set logscale y
set xl "t"
set yl "|y_{num}(t)-y_{anal}(t)|"

set out "Metoda_Eulera_blad.png"
set title "Metoda Eulera - błąd globalny"
p "explicit_Euler_method_0.010000.dat" u 1:4 w lines t "dt = 0.01"   ls 1,\
  "explicit_Euler_method_0.100000.dat" u 1:4 w lines t "dt = 0.1"    ls 2,\
  "explicit_Euler_method_1.000000.dat" u 1:4 w lines t "dt = 1.0"    ls 3,\
  "explicit_Euler_method_0.010000.dat" u 1:4 w lines t "analityczne" ls 4
  
set out "Metoda_RK2_blad.png"
set title "Metoda RK2 - błąd globalny"
p "explicit_RK2_method_0.010000.dat" u 1:4 w lines t "dt = 0.01"   ls 1,\
  "explicit_RK2_method_0.100000.dat" u 1:4 w lines t "dt = 0.1"    ls 2,\
  "explicit_RK2_method_1.000000.dat" u 1:4 w lines t "dt = 1.0"    ls 3,\
  "explicit_RK2_method_0.010000.dat" u 1:4 w lines t "analityczne" ls 4
  
set out "Metoda_RK4_blad.png"
set title "Metoda RK4 - błąd globalny"
p "explicit_RK4_method_0.010000.dat" u 1:4 w lines t "dt = 0.01"   ls 1,\
  "explicit_RK4_method_0.100000.dat" u 1:4 w lines t "dt = 0.1"    ls 2,\
  "explicit_RK4_method_1.000000.dat" u 1:4 w lines t "dt = 1.0"    ls 3,\
  "explicit_RK4_method_0.010000.dat" u 1:4 w lines t "analityczne" ls 4
  
  
  
#unset logscale y
set title "RLC"
set out "RLC_I.png"
set yl "I(t)"
p "RLC_Q_I_factor_0.500000.dat" u 1:2 w lines t "w_V = 0.5 * w_0" 	ls 1,\
  "RLC_Q_I_factor_0.800000.dat" u 1:2 w lines t "w_V = 0.8 * w_0"	ls 2,\
  "RLC_Q_I_factor_1.000000.dat" u 1:2 w lines t "w_V = 1.0 * w_0"	ls 3,\
  "RLC_Q_I_factor_1.200000.dat" u 1:2 w lines t "w_V = 1.2 * w_0"	ls 4
  
set out "RLC_Q.png"
set yl "Q(t)"
p "RLC_Q_I_factor_0.500000.dat" u 1:3 w lines t "w_V = 0.5 * w_0" 	ls 1,\
  "RLC_Q_I_factor_0.800000.dat" u 1:3 w lines t "w_V = 0.8 * w_0"	ls 2,\
  "RLC_Q_I_factor_1.000000.dat" u 1:3 w lines t "w_V = 1.0 * w_0"	ls 3,\
  "RLC_Q_I_factor_1.200000.dat" u 1:3 w lines t "w_V = 1.2 * w_0"	ls 4