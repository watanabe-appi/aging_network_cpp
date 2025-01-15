set term pdfcairo size 10cm, 10cm enhanced font "Helvetica,14"

set output "phase_diagram.pdf"
set size ratio -1
unset key
set xrange [-1.3:3.2]
set yrange [-1.3:3.2]
set xlabel "{/Symbol a}"
set ylabel "{/Symbol b}"

p "exp_finite.dat"   pt 7 ps 1.5    lc "red"\
, "exp_zero.dat"     pt 7 ps 1.5    lc "blue"\
, "power_finite.dat" pt 2 ps 1 lw 5 lc "red" \
, "power_zero.dat"   pt 2 ps 1 lw 5 lc "blue"\
