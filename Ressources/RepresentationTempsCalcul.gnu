set terminal png size 1280,941 crop

set xlabel "Nombre de Chaines"
set ylabel "Temps d'execution"
set title "Temps de reconsitution du reseau en Liste, en Hachage et en Arbre"

set output 'TempsCalculEn3methode.png'
plot "timedata.txt2" using 1:2 with lines title "TempsListeChaine", \
"timedata.txt2" using 1:3 with lines title "TempsHachage", \
"timedata.txt2" using 1:4 with lines title "TempsArbreQuat"

set output "TempsCalculListe.png"
plot "timedata.txt2" using 1:2 with lines title "TempsListeChaine"

set output "TempsCalculHachage.png"
plot "timedata.txt2" using 1:3 with lines title "TempsHachage"

set output "TempsCalculArbre.png"
plot "timedata.txt2" using 1:4 with lines title "TempsArbreQuat"