#!/bin/bash
#launch_tests [nbTests] [nbNodesMin] [nbNodesMax]

ecart=`expr $3 - $2 `
ecart=`expr $ecart / $(($1 - 1)) `

echo "Compilation du programme de creation des fichiers XML..."
cd createXML
make
echo "Lancement de $1 tests..."
for i in $(seq 0 $(($1 - 1)));
do
	echo "Lancement d'un test avec $(($(($i * $ecart)) + $2)) noeuds..."
	./createXML $(($(($i * $ecart)) + $2)) -1
	mv 0.xml ../XMLs/0.xml
	mv 1.xml ../XMLs/1.xml
	mv 2.xml ../XMLs/2.xml
	mv 3.xml ../XMLs/3.xml
	mv 4.xml ../XMLs/4.xml
	mv 5.xml ../XMLs/5.xml
	mv density.txt ../resultats/
	mv topoVisu.txt ../resultats/
	cd ..
	wsnet-run-simulations -d ./XMLs/ ./resultats/ 1
	cd ./createXML
	
	#wsnet -c 0.xml
	#wsnet -c 1.xml
	#wsnet -c 2.xml
	#wsnet -c 3.xml
	#wsnet -c 4.xml
	#wsnet -c 5.xml
done

