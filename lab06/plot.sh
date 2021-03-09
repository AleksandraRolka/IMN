set term png size 600,600


set out "w1.png"
set title "{/:Italic S}({/:Italic it})"
set xl "{/:Italic x}"
set yl "{/:Italic y}"
set zl "{/:Italic V}"



set pm3d map
set palette defined ( 0 "blue", 1 "white", 2 "red" )

set out "w0.png"
set title "{/:Italic n_{x}= n_{y}= 4, eps_{1}= 1, eps_{2}}= 1"
splot "V____n_x=4_n_y=4_eps_1=1.000000_eps_2=1.000000_rhoNull.dat" u 2:3:4 notitle



set out "w1.png"
set title "{/:Italic n_{x}= n_{y}}= 50, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 1"
splot "V____n_x=50_n_y=50_eps_1=1.000000_eps_2=1.000000_rhoNull.dat" u 2:3:4 notitle
set out "w2.png"
set title "{/:Italic n_{x}= n_{y}}= 100, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 1"
splot "V____n_x=100_n_y=100_eps_1=1.000000_eps_2=1.000000_rhoNull.dat" u 2:3:4 notitle
set out "w3.png"
set title "{/:Italic n_{x}= n_{y}}= 200, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 1"
splot "V____n_x=200_n_y=200_eps_1=1.000000_eps_2=1.000000_rhoNull.dat" u 2:3:4 notitle


set cbrange [-0.8:0.8]
 
set out "w4.png"
set title "{/:Italic n_{x}= n_{y}}= 100, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 1"
splot "V____n_x=100_n_y=100_eps_1=1.000000_eps_2=1.000000.dat" u 2:3:4 notitle
set out "w5.png"
set title "{/:Italic n_{x}= n_{y}}= 100, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 2"
splot "V____n_x=100_n_y=100_eps_1=1.000000_eps_2=2.000000.dat" u 2:3:4 notitle
set out "w6.png"
set title "{/:Italic n_{x}= n_{y}}= 100, {/:Italic eps_{1}}= 1, {/:Italic eps_{2}}= 1" 
splot "V____n_x=100_n_y=100_eps_1=1.000000_eps_2=10.000000.dat" u 2:3:4 notitle

