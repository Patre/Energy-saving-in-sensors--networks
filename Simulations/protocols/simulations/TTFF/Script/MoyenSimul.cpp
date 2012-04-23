#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

	//LES ARGUMENT
	if (argc < 3) {
		cout << "Manque paramatres\n\tNombre De Simulation\n\tid Protocole consulter IDprotocole.tds (0,1,2,3,4,5)\n";
		return 0;
	}
	if (argc > 3) {
		cout
				<< "BCP de paramaitres \nil faut un paramatre de Nombre De Simulation seulement \n";
		return 0;
	}
	int nbrSim = atoi(argv[1]);
	cout << "FICHIER DE SORTI TTFF(time to first fail)\nNOMBE DE SIMULATION IS "
			<< nbrSim << endl;

	//FICHIER DE SORTI
	ofstream sorti(argv[2], ios::out | ios::trunc); // ouverture en écriture avec effacement du fichier ouvert

	if (!sorti)
		cerr << "Impossible d'ouvrir le fichier !" << endl;

	//Les entries
	double time;

	for (int i = 0; i < nbrSim; i++) {
		char s[100];
		sprintf(s, "%s.xml/simulation-%d/lifetime",argv[2],i);
		string filename(s);

		ifstream fichier(filename.c_str(), ios::in); // on ouvre en lecture
		if (fichier) {
			string ligne, ligneP;

			while (getline(fichier, ligne)) // tant que l'on peut mettre la ligne dans "contenu"
			{
				ligneP = ligne;
				cout << ligne << endl; // on l'affiche
			}
			sorti << "N100R30 " << ligneP << endl;
			double timeTmp;
			sscanf(ligneP.c_str(), "%lf", &timeTmp);
			cout << timeTmp << endl;
			time += timeTmp;
			fichier.close();
		} else
			cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
	sorti << argv[2] <<" "<<time / (double) nbrSim << endl;
	sorti.close();
	return 0;
}
