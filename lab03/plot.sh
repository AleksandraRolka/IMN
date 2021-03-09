set term png #size 1000,1000

set style line 1  lc rgb '#9999ff' lt 1 lw 2
set style line 2  lc rgb '#00ffcc' lt 1 lw 2 


set xl "{/:Italic t}"


set out "w1_Metoda_RK2.png"
set title "Zależność {/:Italic x} od {/:Italic t}"
set yl "{/:Italic x}({/:Italic t})"
p "Metoda_RK2_TOL_1.dat" u 1:3 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_RK2_TOL_2.dat" u 1:3 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2


set out "w2_Metoda_RK2.png"
set title "Zależność {/:Italic v} od {/:Italic t}"
set yl "{/:Italic v}({/:Italic t})"
p "Metoda_RK2_TOL_1.dat" u 1:4 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_RK2_TOL_2.dat" u 1:4 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2

set out "w3_Metoda_RK2.png"
set title "Zależność {/:Italic dt} od {/:Italic t}"
set yl "{/:Italic dt}({/:Italic t})"
p "Metoda_RK2_TOL_1.dat" u 1:2 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_RK2_TOL_2.dat" u 1:2 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2
  
set out "w4_Metoda_RK2.png"
set title "Zależność {/:Italic v} od {/:Italic x}"
set xl "{/:Italic x}"
set yl "{/:Italic v}({/:Italic x})"
set key inside right bottom;
p "Metoda_RK2_TOL_1.dat" u 3:4 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_RK2_TOL_2.dat" u 3:4 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2



set out "w5_Metoda_trapezow.png"
set title "Zależność {/:Italic x} od {/:Italic t}"
set yl "{/:Italic x}({/:Italic t})"
set key inside right top;
p "Metoda_trapezow_TOL_1.dat" u 1:3 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_trapezow_TOL_2.dat" u 1:3 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2

set out "w6_Metoda_trapezow.png"
set title "Zależność {/:Italic v} od {/:Italic t}"
set yl "{/:Italic v}({/:Italic t})"
p "Metoda_trapezow_TOL_1.dat" u 1:4 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_trapezow_TOL_2.dat" u 1:4 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2

set out "w7_Metoda_trapezow.png"
set title "Zależność {/:Italic dt} od {/:Italic t}"
set yl "{/:Italic dt}({/:Italic t})"
p "Metoda_trapezow_TOL_1.dat" u 1:2 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_trapezow_TOL_2.dat" u 1:2 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2
  
set out "w8_Metoda_trapezow.png"
set title "Zależność {/:Italic v} od {/:Italic x}"
set xl "{/:Italic x}"
set yl "{/:Italic v}({/:Italic x})"
set key inside right bottom;
p "Metoda_trapezow_TOL_1.dat" u 3:4 w lines t "{/:Italic TOL} = 10^{-2}"   ls 1,\
  "Metoda_trapezow_TOL_2.dat" u 3:4 w lines t "{/:Italic TOL} = 10^{-5}"   ls 2
