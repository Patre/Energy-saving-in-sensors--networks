#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

	//LES ARGUMENT
	if (argc < 2) {
		cout << "Manque paramatre de Nombre De Protocole \n";
		return 0;
	}
	if (argc > 2) {
		cout
				<< "BCP de paramaitres \nil faut un paramatre de Nombre De protocole seulement \n";
		return 0;
	}
	int nbrPro = atoi(argv[1]);
	cout << "FICHIER DE SORTI TTFF(time to first fail)\nNOMBE OF PROTOCOLS IS "
			<< nbrPro << endl;

	//FICHIER DE SORTI
	ofstream sorti("ttff", ios::out | ios::trunc); // ouverture en écriture avec effacement du fichier ouvert

	if (!sorti)
		cerr << "Impossible de creer le fichier !" << endl;

	//Les entries

	for (int i = 0; i < nbrPro; i++) {
		char s[100];
		sprintf(s, "results/%d", i,i);
		string filename(s);

		ifstream fichier(filename.c_str(), ios::in); // on ouvre en lecture
		if (fichier) {
			string ligne, ligneP;

			while (getline(fichier, ligne)) // tant que l'on peut mettre la ligne dans "contenu"
			{
				ligneP = ligne;
				cout << ligne << endl; // on l'affiche
			}
			sorti << ligneP << endl;
			fichier.close();
		} else
			cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	sorti.close();
	return 0;
}
