#!/bin/bash
#launch_tests [nbTests] [nbNodesMin] [nbNodesMax] [topoType]

ecart=`expr $3 - $2 `
ecart=`expr $ecart / $(($1 - 1)) `

if [ $4 == 0 ]
then
	resFile="topologie_alea"
else
	resFile="topologie_hole"
fi

echo "Compilation du programme de creation des fichiers XML..."
cd ./createXML
make
echo "Lancement de $1 tests..."
for i in $(seq 0 $(($1 - 1)));
do
	echo "Lancement d'un test avec $(($(($i * $ecart)) + $2)) noeuds..."
	./createXML $(($(($i * $ecart)) + $2)) -1 $4
	for i in $(seq 0 5);
	do
		mv "$i".xml ../XMLs/"$i".xml
	done
	mv topologie.txt ../resultats/
	mv topoVisu.txt ../resultats/
	cd ..
	wsnet-run-simulations -d ./XMLs/ ./resultats/ 1
	cd resultats
	cat topologie.txt | awk 'NR == 2 {printf "%s ; ", $1 >> "ttff.txt"; printf "%s ; ", $1 >> "pcn.txt"; printf "%s ; ", $1 >> "lc.txt"}'
	for i in $(seq 0 5);
	do
		cat "$i".xml/simulation-0/lifetime | awk '{if($1 == "TTFF") {printf "%.1f ", $2 >> "ttff.txt"}; if($1 == "PCN") {printf "%.1f ", $2 >> "pcn.txt"}; if($1 == "LC") {printf "%.1f ", $2 >> "lc.txt"}; }'
		printf "; " >> "ttff.txt"
		printf "; " >> "pcn.txt"
		printf "; " >> "lc.txt"
	done
	echo "" >> ttff.txt
	echo "" >> pcn.txt
	echo "" >> lc.txt
	cd ../createXML
done
cd ../resultats
cat ttff.txt >> "$resFile"/ttff.txt
cat pcn.txt >> "$resFile"/pcn.txt
cat lc.txt >> "$resFile"/lc.txt
for i in $(seq 0 5);
do
		rm -r "$i".xml
done
rm lc.txt
rm pcn.txt
rm ttff.txt
rm topologie.txt
rm topoVisu.txt
cd ..


