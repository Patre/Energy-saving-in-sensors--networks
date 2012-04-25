
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include<iomanip>
#include<fstream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
#define DBLMAX 99999999999;
#define RANGE 30


struct pos
{
	double x;
	double y;
};

double distanceEuc(pos a, pos b)
{
	return sqrt(pow(a.x-b.x, 2)+pow(a.y-b.y, 2));
}

void writeFile(string  nomFichier, int nbNoeuds, pos tabNoeuds[], int algo)
{
    char s[200];
    strcpy(s,nomFichier.c_str());

    mkdir(s,01777);
    sprintf(s,"%s/%d.xml",s,algo);

	ofstream sortie(s, ios::out|ios::trunc);
	
	if (!sortie)
		cerr << "Impossible de creer le fichier !" << endl;
	
	sortie << "<worldsens xmlns=\"http://worldsens.citi.insa-lyon.fr\">" << endl;
	sortie << "<simulation nodes=\"" << nbNoeuds << "\" duration=\"10000s\" x=\"200\" y=\"200\" z=\"0\"/>" << endl;
	sortie << "<entity name=\"range\" library=\"propagation_range\"> <init range=\"" << RANGE << "\"/> </entity>" << endl;
	sortie << "<entity name=\"interf\" library=\"interferences_none\"></entity>" << endl;
	sortie << "<entity name=\"none\" library=\"modulation_none\"></entity>" << endl;
	sortie << "<entity name=\"omnidirectionnal\" library=\"antenna_omnidirectionnal\"> ";
	sortie << "<default loss=\"0\" angle-xy=\"random\" angle-z=\"0\"/> </entity>" << endl;
	sortie << "<entity name=\"radio\" library=\"radio_half1d\"> <default sensibility=\"-92\" T_b=\"727\" dBm=\"10\" channel=\"0\" modulation=\"none\"/> </entity>" << endl;
	sortie << "<entity name=\"static-pos\" library=\"mobility_static\"> <default x=\"random\" y=\"random\" z=\"0.0\"/> </entity>" << endl;
	sortie << "<entity name=\"app-sensor\" library=\"application_broadcast\"> <init period=\"10s\" debut=\"30s\" node=\"-1\" debug=\"0\"/> <default /> </entity>" << endl;
	sortie << "<entity name=\"routing-protocole\" library=\"";
	switch (algo) {
		case 0:
			sortie << "routing_flood";
			break;
		case 1:
			sortie << "routing_rbop";
			break;
		case 2:
			sortie << "routing_lbop";
			break;
		case 3:
			sortie << "routing_bip";
			break;
		case 4:
			sortie << "routing_lbip";
			break;
		case 5:
			sortie << "routing_dlbip";
			break;
			
		default:
			break;
	}
	sortie << "\"> <init alpha=\"4\" c=\"100000000\" eps=\"0.01\" debug=\"0\"/> <default /> </entity>" << endl;
	sortie << "<entity name=\"mac\" library=\"mac_macter\"> <init  debit=\"10\"  alpha=\"4\" c=\"100000000\" debug=\"0\"/> <default range =\"" << RANGE << "\"/> </entity>" << endl;
	sortie << "<entity name=\"theorie\" library=\"energy_theorie\"> <init  debug=\"0\"/> <default energy=\"5000000000\" rx=\"1\"  tx=\"1\" /> </entity>" << endl;
	sortie << "<environment> <propagation entity=\"range\" range=\"" << RANGE << "\"/> <interferences entity=\"interf\"/> <modulation entity=\"none\"/> </environment>" << endl;
	sortie << "<bundle name=\"sensor\" worldsens=\"false\" default=\"true\" birth=\"0\"> <mobility entity=\"static-pos\"/> <energy entity=\"theorie\" /> <antenna entity=\"omnidirectionnal\"> <up entity=\"radio\"/> </antenna> <with entity=\"radio\"> <up entity=\"mac\"/> <down entity=\"omnidirectionnal\"/> </with> <with entity=\"mac\"> <up entity=\"routing-protocole\"/> <down entity=\"radio\"/> </with> <with entity=\"routing-protocole\"> <up entity=\"app-sensor\"/> <down entity=\"mac\"/> </with> <with entity=\"app-sensor\"> <down entity=\"routing-protocole\"/> </with> </bundle>" << endl;
	
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		sortie << "<node id=\"" << i << "\"> <for entity=\"static-pos\" x=\"" << tabNoeuds[i].x << "\" y=\"" << tabNoeuds[i].y << "\" z=\"0.0\" /> </node>" << endl;
	}
	sortie << "</worldsens>" << endl;
	
	sortie.close();
}

void createAleaNodes(int nbNoeuds, pos tabNoeuds[])
{
	srand(time(NULL));
	int connexe;
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		connexe = 0;
		while(!connexe)
		{
			tabNoeuds[i].x = (double)rand()/(double)RAND_MAX * 200;
			tabNoeuds[i].y = (double)rand()/(double)RAND_MAX * 200;
			for(int j = 0 ; j < i ; j++)
			{
				if(distanceEuc(tabNoeuds[i], tabNoeuds[j]) < RANGE)
				{
					connexe = 1;
					break;
				}
			}
			if(i == 0)
				connexe = 1;
		}
		
		//cout << '(' << tabNoeuds[i].x << ';' << tabNoeuds[i].y << ')' << endl;
	}
}

double computeAvDegree(int nbNoeuds, pos* tabNoeuds)
{
	int degrees[nbNoeuds];
	float avDegree = 0;
	
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		degrees[i] = 0;
	}
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		for(int j = i+1 ; j < nbNoeuds ; j++)
		{
			if(i != j && distanceEuc(tabNoeuds[i], tabNoeuds[j]) < RANGE)
			{
				degrees[i]++;
				degrees[j]++;
			}
		}
	}
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		//cout << "Degré de " << i << " = " << degrees[i] << endl;
		avDegree += degrees[i];
	}
	avDegree /= (2.0*nbNoeuds);
	cout << "Degré moyen = " << avDegree << endl;
	
	return avDegree;
}

double computeDensity(int N,pos *tabNoeuds)
{
    int Degree=0;
    int i,j,k;
	
	
    //WRL ALGO
    double W[N][N];
    double WP[N][N];
	
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            W[i][j]=DBLMAX;
	
	
    double dist;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            dist=distanceEuc(tabNoeuds[i], tabNoeuds[j]);
            if(i!=j && dist<= RANGE)
            {
                Degree++;
                W[i][j]=1;
            }
            else if(i==j)
                W[i][j]=0;
        }
	
    for(k=0;k<N;k++)
    {
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                WP[i][j]=W[i][j];
		
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
            {
                W[i][j] = min(W[i][j],W[i][k]+W[k][j]);
            }
    }
	
	
    //CALCUL DENSITé
    double degreeMo=(double)Degree/(double)2;
    degreeMo=degreeMo /(double)N;
	
	
    double diametre=0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            if(W[i][j]>diametre) diametre=W[i][j];
	
	
	
    double densite=degreeMo/diametre;
    //printf("Degre moyen : %lf\nDiametre : %lf\nDensite : %lf\n",degreeMo,diametre,densite);
	
    /*if(diametre == DBLMAX)
    {
        printf("End Of Simulation (un node est isolé)\n");
        //ENDERR("un NODE est ISOLe\n");
        //end_simulation();
        return -1;
    }*/
	
    return densite;
    //END("%lf",densite);//
	
}


void createVisuGraph(int nbNoeuds, pos* tabNoeuds)
{
	ofstream topo("topoVisu", ios::out|ios::trunc);
	
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		topo << "N: " << i << " " << tabNoeuds[i].x << " " << tabNoeuds[i].y << endl;
	}
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		for(int j = 0 ; j < nbNoeuds ; j++)
		{
			if(i != j && distanceEuc(tabNoeuds[i], tabNoeuds[j]) < RANGE)
				topo << "G: " << i << " " << i << " " << j << endl;
		}
	}
	
	topo.close();
}

vector<int>* getArticulationNodes(int nbNoeuds, pos* tabNoeuds)
{
	ofstream topo("grapheForJava", ios::out|ios::trunc);
	topo << nbNoeuds << endl;
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		for(int j = 0 ; j < nbNoeuds ; j++)
		{
			if(i != j && distanceEuc(tabNoeuds[i], tabNoeuds[j]) < RANGE)
				topo << i << " " << j << endl;
		}
	}
	topo.close();
	
	system("cd ArticPointDFS\njava ArticPointDFS ../grapheForJava\ncd ..\n");
	
	vector<int>* articulations = new vector<int>;
	int aPoint, num;
	ifstream topo2("grapheForJava", ios::in);
	topo2 >> num;
	for(int i = 0 ; i < num ; i++)
	{
		topo2 >> aPoint;
		articulations->push_back(aPoint);
	}
	topo2.close();
	system("rm grapheForJava\n");
	return articulations;
}


int main(int argc, char* argv[], char* env[])
{
	// args : nomFichier, nbNoeuds, algo
	if(argc < 4)
	{
                cerr << "createXML [filename] [nodesNb] [Nbr ALGO]" << endl;
		return -1;
	}
	int nbNoeuds = atoi(argv[2]);
	pos* tabNoeuds = new pos[nbNoeuds];
	createAleaNodes(nbNoeuds, tabNoeuds);
	computeAvDegree(nbNoeuds, tabNoeuds);


        double densite=computeDensity(nbNoeuds,tabNoeuds);
        char s[200];
        char s1[200];

        sprintf(s1,"%lf/results",densite);
        sprintf(s,"%lf",densite);
        mkdir(s,01777);
        mkdir(s1,01777);
        sprintf(s,"%s/XMLs",s);

        for(int i=0;i<atoi(argv[3]);i++)
                writeFile(s, nbNoeuds, tabNoeuds, i);


	createVisuGraph(nbNoeuds, tabNoeuds);

	/*vector<int>* articulations = getArticulationNodes(nbNoeuds, tabNoeuds);
	cout << "Points d'articulation : ";
	for(unsigned int i = 0 ; i < articulations->size() ; i++)
		cout << articulations->at(i) << " ";
	cout << endl;
	delete articulations;//*/
	delete tabNoeuds;
	
	return 0;
}


