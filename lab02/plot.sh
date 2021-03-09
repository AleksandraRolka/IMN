set term png #size 1000,1000

set style line 1  lc rgb '#80bfff' lt 1 lw 3
set style line 2  lc rgb '#c6ff1a' lt 1 lw 3 


set xl "Czas {/:Italic t}"
set yl "Liczba osób {/:Italic u}({/:Italic t}), {/:Italic z}({/:Italic t})"


set out "Metoda_Picarda.png"
set title "Dynamika rozprzestrzeniania się choroby zakaźnej (model SIS)\n- metoda trapezów z iteracją Picarda"
p "Picard_method.dat" u 1:2 w lines t "{/:Italic u}({/:Italic t}) - nosiciele choroby"  ls 1,\
  "Picard_method.dat" u 1:3 w lines t "{/:Italic z}({/:Italic t}) - osoby zdrowe"  ls 2


set out "Metoda_Newtona.png"
set title "Dynamika rozprzestrzeniania się choroby zakaźnej (model SIS)\n- metoda trapezów z iteracją Newtona"
p "Newton_method.dat" u 1:2 w lines t "{/:Italic u}({/:Italic t}) - nosiciele choroby"  ls 1,\
  "Newton_method.dat" u 1:3 w lines t "{/:Italic z}({/:Italic t}) - osoby zdrowe"  ls 2

set out "Niejawna_metoda_RK2.png"
set title "Dynamika rozprzestrzeniania się choroby zakaźnej (model SIS)\n- niejawna metoda RK2"
p "RK2_method.dat" u 1:2 w lines t "{/:Italic u}({/:Italic t}) - nosiciele choroby"  ls 1,\
  "RK2_method.dat" u 1:3 w lines t "{/:Italic z}({/:Italic t}) - osoby zdrowe"  ls 2

