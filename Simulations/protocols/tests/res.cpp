#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {

	//LES ARGUMENT
	if (argc < 2) {
		cout << "Manque paramatre de Nombre Des Noueds \n";
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
	ofstream sorti("lifetimeconnexe", ios::out | ios::trunc); // ouverture en écriture avec effacement du fichier ouvert

	if (!sorti)
		cerr << "Impossible de creer le fichier !" << endl;

	//Les entries
	char s[100];
	sprintf(s, "lifetime");
	string filename(s);

	ifstream fichier(filename.c_str(), ios::in); // on ouvre en lecture
	if (fichier) {
		string ligne, ligneP;
		int nbr =nbrPro;
		int node = 0;
		int time = 0;
		bool test=true;
		while (getline(fichier, ligne) && test) // tant que l'on peut mettre la ligne dans "contenu"
		{
			if(ligne == "----")
			{
				cout<<"prochaine Broadcast"<<endl;
				cout<< time <<" "<< node<<" "<<nbr<<" "<<nbrPro<<endl;
				if(nbr!=nbrPro)
					test=false;
				nbr=0;
			}
			else
			{
				nbr++;
				double timeF;
				sscanf(ligne.c_str(),"%lf %d",&timeF,&node);
				time=(int) timeF;
			}
						
			if(test) ligneP = ligne;
			
			//cout << ligne << endl; // on l'affiche
		}
		sorti << ligneP << endl;
		fichier.close();
	} else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	
	sorti.close();
	return 0;
}