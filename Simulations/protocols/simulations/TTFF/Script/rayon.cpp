#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

	//LES ARGUMENT

	//Les entries
	double range=0;
	

		ifstream fichier("rayon", ios::in); // on ouvre en lecture
		if (fichier) {
			string ligne;

			while (getline(fichier, ligne)) // tant que l'on peut mettre la ligne dans "contenu"
			{
			double rangeTmp;
			sscanf(ligne.c_str(), "%lf", &rangeTmp);
			if(rangeTmp>range) range=rangeTmp;	
			}
			
			fichier.close();
		} else
			cerr << "Impossible d'ouvrir le fichier !" << endl;

		cout << "RANGE "<<range<<endl;
		return 0;
}
