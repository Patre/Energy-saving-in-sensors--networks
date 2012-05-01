#!/bin/bash
#launch_tests [nbTests] [nbNodes] [topoType]

WHICH=$(which "launch_tests")
CODE_RETOUR=$?

if [ $CODE_RETOUR -eq 0 ]; then

    CHEMIN=$(dirname $WHICH)
else

    CHEMIN=$(dirname $0)
    if [ "$CHEMIN" = "." ]; then
        CHEMIN=$(pwd)
    else 
        if [ "${CHEMIN:0:2}" = "./" ]; then
            CHEMIN="$(pwd)/${CHEMIN:2}"
        fi
    fi
    
fi

export JAVAPATH="$CHEMIN"/ArticPointDFS
export TOPOPATH="$CHEMIN"/resultats

if [ $3 -eq 0 ]
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
	echo "Lancement du test $i..."
	./createXML $2 -1 $3
	for j in $(seq 0 5);
	do
		mv "$j".xml ../XMLs/"$j".xml
	done
	mv topoVisu.txt ../resultats/
	mv topologie.txt ../resultats/
	cd ..
	if [ $i -gt 0 ]
	then
		rm "XMLs/0.xml"
	fi
	wsnet-run-simulations -d ./XMLs/ ./resultats/ 1
	cd resultats
	for j in $(seq 0 0);
	do
		cat topologie.txt | awk 'NR == 2 {printf "%s ; ", $1 >> "ttff.txt"; printf "%s ; ", $1 >> "pcn.txt"; printf "%s ; ", $1 >> "lc.txt"}'
		cat "0.xml"/simulation-"$j"/lifetime | awk '{if($1 == "TTFF") {printf "%.1f ", $2 >> "ttff.txt"; printf "%.1f ", $2 >> "pcn.txt"; printf "%.1f ", $2 >> "lc.txt"}; }'
		printf "; " >> "ttff.txt"
		printf "; " >> "pcn.txt"
		printf "; " >> "lc.txt"
		for k in $(seq 1 5);
		do
			cat "$k".xml/simulation-"$j"/lifetime | awk '{if($1 == "TTFF") {printf "%.1f ", $2 >> "ttff.txt"}; if($1 == "PCN") {printf "%.1f ", $2 >> "pcn.txt"}; if($1 == "LC") {printf "%.1f ", $2 >> "lc.txt"}; }'
			printf "; " >> "ttff.txt"
			printf "; " >> "pcn.txt"
			printf "; " >> "lc.txt"
		done
		echo "" >> ttff.txt
		echo "" >> pcn.txt
		echo "" >> lc.txt
	done

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
cd ..