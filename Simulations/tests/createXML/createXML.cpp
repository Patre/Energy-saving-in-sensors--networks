
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <float.h>

using namespace std;

#define RANGE 30
#define WIDTH 1000
#define DUREE 1000000
#define PERIOD_BROADCAST 2


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
	nomFichier += ".xml";
	ofstream sortie(nomFichier.c_str(), ios::out|ios::trunc);
	
	if (!sortie)
		cerr << "Impossible de creer le fichier !" << endl;
	
	sortie << "<worldsens xmlns=\"http://worldsens.citi.insa-lyon.fr\">" << endl;
	sortie << "<simulation nodes=\"" << nbNoeuds << "\" duration=\"" << DUREE << "s\" x=\"" << WIDTH << "\" y=\"" << WIDTH << "\" z=\"0\"/>" << endl;
	sortie << "<entity name=\"range\" library=\"propagation_range\"> <init range=\"" << RANGE << "\"/> </entity>" << endl;
	sortie << "<entity name=\"interf\" library=\"interferences_none\"></entity>" << endl;
	sortie << "<entity name=\"none\" library=\"modulation_none\"></entity>" << endl;
	sortie << "<entity name=\"omnidirectionnal\" library=\"antenna_omnidirectionnal\"> ";
	sortie << "<default loss=\"0\" angle-xy=\"random\" angle-z=\"0\"/> </entity>" << endl;
	sortie << "<entity name=\"radio\" library=\"radio_half1d\"> <default sensibility=\"-92\" T_b=\"727\" dBm=\"10\" channel=\"0\" modulation=\"none\"/> </entity>" << endl;
	sortie << "<entity name=\"static-pos\" library=\"mobility_static\"> <default x=\"random\" y=\"random\" z=\"0.0\"/> </entity>" << endl;
	sortie << "<entity name=\"app-sensor\" library=\"application_broadcast\"> <init period=\"2s\" debut=\"30s\" node=\"-1\" debug=\"0\"/> <default /> </entity>" << endl;
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
	sortie << "<entity name=\"theorie\" library=\"energy_theorie\"> <init  debug=\"0\"/> <default energy=\"5000000000\" rx=\"1\"  tx=\"1\" range=\"" << RANGE << "\" PRNmin=\"75.0\" alpha=\"4\" c=\"100000000\"/> </entity>" << endl;
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

void createHoleNodes(int nbNoeuds, pos tabNoeuds[])
{
	/*int connexe;
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
	}*/
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
		//cout << "DegrE de " << i << " = " << degrees[i] << endl;
		avDegree += degrees[i];
	}
	avDegree /= (2.0*nbNoeuds);
	cout << "\tDegre moyen = " << avDegree << endl;
	
	return avDegree;
}

double computeDiameter(int N, pos* tabNoeuds)
{
    int i,j,k;
	
	
    //WRL ALGO
    double W[N][N];
    double WP[N][N];
	
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            W[i][j]= DBL_MAX;
	
	
    double dist;
    for(i=0 ; i<N ; i++)
        for(j=0 ; j<N ; j++)
        {
            dist=distanceEuc(tabNoeuds[i], tabNoeuds[j]);
            if(i!=j && dist<= RANGE)
            {
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
	
    double diametre=0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            if(W[i][j]>diametre) diametre=W[i][j];
	cout << "\tDiametre : " << diametre << endl;
	
	return diametre;
}

double computeDensity(int nbNoeuds, pos* tabNoeuds)
{
	ofstream densityFile("topologie.txt", ios::out|ios::app);
	double densite = computeAvDegree(nbNoeuds, tabNoeuds)/computeDiameter(nbNoeuds, tabNoeuds);
	densityFile << densite << endl;
	printf("\tDensite : %lf\n", densite);
	densityFile.close();
	
	return densite;
}


void createVisuGraph(int nbNoeuds, pos* tabNoeuds)
{
	ofstream topo("topoVisu.txt", ios::out|ios::trunc);
	
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		topo << "N: " << i << " " << tabNoeuds[i].x << " " << tabNoeuds[i].y << endl;
	}
	for(int i = 0 ; i < nbNoeuds ; i++)
	{
		for(int j = 0 ; j < nbNoeuds ; j++)
		{
			if(i != j && distanceEuc(tabNoeuds[i], tabNoeuds[j]) < RANGE)
				topo << "G: " << i << " " << j << endl;
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
	
	system("cd ../ArticPointDFS\njava ArticPointDFS ../createXML/grapheForJava\ncd ../createXML\n");
	
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

void createNodes(int topoType, int nbNoeuds, pos* tabNoeuds)
{
	switch (topoType)
	{
		case 0:
			createAleaNodes(nbNoeuds, tabNoeuds);
			break;
			
		case 1:
			createHoleNodes(nbNoeuds, tabNoeuds);
			break;
			
		default:
			break;
	}
}

void createBroadcastingNodes(int nbNoeuds)
{
	ofstream topo("topologie.txt", ios::out|ios::app);
	for(int i = 0 ; i < DUREE ; i+=PERIOD_BROADCAST)
	{
		topo << i << rand() % nbNoeuds << endl;
	}
	topo.close();
}


int main(int argc, char* argv[], char* env[])
{
	srand(time(NULL));
	// args : nbNoeuds, algo
	if(argc < 4)
	{
		cerr << "createXML [nodesNb] [algoNb] [topoType]" << endl;
		return -1;
	}
	int nbNoeuds = atoi(argv[1]);
	pos* tabNoeuds = new pos[nbNoeuds];
	ofstream topo("topologie.txt", ios::out|ios::trunc);
	topo << argv[3] << endl;
	topo.close();
	
	if(atoi(argv[2]) == -1)
	{
		createNodes(atoi(argv[3]), nbNoeuds, tabNoeuds);
		computeDensity(nbNoeuds, tabNoeuds);
		for(int i = 0 ; i <= 5 ; i++)
		{
			//cout << "Creation du fichier XML " << i << "..." << endl;
			char nomFichier[2] = {48+i, 0};
			writeFile(nomFichier, nbNoeuds, tabNoeuds, i);
		}
		createVisuGraph(nbNoeuds, tabNoeuds);
	}
	else
	{
		//cout << "Creation du fichier XML " << argv[2] << " : " << endl;
		createNodes(atoi(argv[3]), nbNoeuds, tabNoeuds);
		computeDensity(nbNoeuds, tabNoeuds);
		writeFile(argv[2], nbNoeuds, tabNoeuds, atoi(argv[2]));
		createVisuGraph(nbNoeuds, tabNoeuds);
	}
	vector<int>* articulations = getArticulationNodes(nbNoeuds, tabNoeuds);
	/*cout << "Points d'articulation : ";
	for(unsigned int i = 0 ; i < articulations->size() ; i++)
		cout << articulations->at(i) << " ";
	cout << endl;*/
	delete articulations;
	delete tabNoeuds;
	
	return 0;
}


