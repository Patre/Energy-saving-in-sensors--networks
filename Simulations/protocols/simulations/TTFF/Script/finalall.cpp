#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

	//LES ARGUMENT
	if (argc < 2) {
		cout << "Dossier contenant TTFF \n";
		return 0;
	}
	if (argc > 2) {
		cout
				<< "BCP de paramaitres \nil faut un paramatre de Nombre De protocole seulement \n";
		return 0;
	}
	cout << "FICHIER D'ENTREE(time to first fail) is "
			<< argv[1] << endl;

	//FICHIER DE SORTI
	ofstream sorti("ttffFinal", ios::out | ios::app); // ouverture en écriture avec effacement du fichier ouvert

	if (!sorti)
		cerr << "Impossible de creer le fichier !" << endl;
	sorti << endl;
	//Les entries

		char s[100];
		sprintf(s, "%s/ttff", argv[1]);
		string filename(s);

		ifstream fichier(filename.c_str(), ios::in); // on ouvre en lecture
		if (fichier) {
			string ligne;

			while (getline(fichier, ligne)) // tant que l'on peut mettre la ligne dans "contenu"
			{
				sorti << argv[1] <<" "<< ligne << endl; // on l'affiche
			}
			fichier.close();
		} else
			cerr << "Impossible d'ouvrir le fichier !" << endl;
	sorti.close();
	return 0;
}
