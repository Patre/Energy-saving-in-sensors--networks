#include <iostream>
#include <fstream>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char ** argv)
{
    char s[20];

    if(argc != 3)
    {
        cout << "il faut 2 argument \t1: le neud a extraire\n \t\t\t2:le fichier de sortie\n";
        return -1;
    }

    strcpy(s,argv[2]);
    FILE *energ;
    energ=fopen(s,"w");

    int node_= atoi(argv[1]);
    ifstream fichier("energy", ios::in);  // on ouvre en lecture
    if(fichier)  // si l'ouverture a fonctionné
    {
            string ligne;
            while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
            {
                    char type;
                    int node;
                    double time, energy;
                    sscanf(ligne.c_str(),"%d %c %lf %lf",&node,&type,&time,&energy);
                    if(node == node_)
                        fprintf(energ,"%lf %lf\n",time,energy);
            }
            fichier.close();
    }
    fclose(energ);//*/

   return 1;
}
