
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <float.h>

using namespace std;

class WithComma: public numpunct<char> // class for decimal numbers with comma
{
protected: char do_decimal_point() const { return ','; } // override the function that gives the decimal separator
};

class WithPoint: public numpunct<char> // class for decimal numbers with comma
{
protected: char do_decimal_point() const { return '.'; } // override the function that gives the decimal separator
};

int main(int argc, char* argv[], char* env[])
{
	if(argc < 2)
	{
		cerr << "lissage_resultats [file_path]" << endl;
		return -1;
	}
	
	locale locComma(locale(), new WithComma);
	locale locPoint(locale(), new WithPoint);
	ifstream donnees(argv[1], ios::in);
	ofstream out("lissage.txt", ios::out|ios::trunc);
	vector<double> densite, flood, rbop, lbop, bip, lbip, dlbip;
	double d, f, r, lbo, b, lbi, dl;
	unsigned int i = 0;
	
	cout << endl << "Lecture du fichier " << argv[1] << "..." << endl;
	while(!donnees.eof())
	{
		while(donnees.good())
		{
			donnees.imbue(locPoint);
			donnees >> d;
			donnees.ignore(1, ';');
			
			donnees.imbue(locComma);
			donnees >> f;
			donnees.ignore(1, ';');
			
			donnees >> r;
			donnees.ignore(1, ';');
			rbop.push_back(r);
			
			donnees >> lbo;
			donnees.ignore(1, ';');
			
			donnees >> b;
			donnees.ignore(1, ';');
			
			donnees >> lbi;
			donnees.ignore(1, ';');
			
			donnees >> dl;
			donnees.ignore(1, ';');
			
			if(donnees.good())
			{
				densite.push_back(d);
				flood.push_back(f);
				lbop.push_back(lbo);
				bip.push_back(b);
				lbip.push_back(lbi);
				dlbip.push_back(dl);
			}
			
			i++;
		}
		if(!donnees.eof())
		{
			donnees.clear();
			char temp[256];
			donnees.getline(temp, 256);
		}
	}
	donnees.close();
	cout << "Lecture du fichier terminee (" << i << " lignes)." << endl;
	
	
	cout << "Calcul des statistiques..." << endl;
	
	double densMin = DBL_MAX, densMax = DBL_MIN;
	for(i = 0 ; i < densite.size() ; i++)
	{
		if(densite[i] < densMin)
			densMin = densite[i];
		if(densite[i] > densMax)
			densMax = densite[i];
	}
	cout << densMin << " <= densite <= " << densMax << " ; " << densite.size() << " valeurs" << endl;
	
	cout << "Valeurs lissees : " << endl;
	vector<double> densiteLisse, floodLisse, rbopLisse, lbopLisse, bipLisse, lbipLisse, dlbipLisse;
	vector<int> nbValeurs;
	double arrondi = floor(densMin * 10)/10.0, max = (floor(densMax*10)+1)/10.0;
	for(i = 0 ; arrondi < max+0.05 ; i++)
	{
		densiteLisse.push_back(arrondi);
		floodLisse.push_back(0);
		rbopLisse.push_back(0);
		lbopLisse.push_back(0);
		bipLisse.push_back(0);
		lbipLisse.push_back(0);
		dlbipLisse.push_back(0);
		nbValeurs.push_back(0);
		for(unsigned int j = 0 ; j < densite.size() ; j++)
		{
			if(densite[j] >= arrondi-0.025 && densite[j] < arrondi+0.025)
			{
				floodLisse[i] += flood[j];
				rbopLisse[i] += rbop[j];
				lbopLisse[i] += lbop[j];
				bipLisse[i] += bip[j];
				lbipLisse[i] += lbip[j];
				dlbipLisse[i] += dlbip[j];
				nbValeurs[i]++;
			}
		}
		if(nbValeurs[i] == 0)
		{
			floodLisse[i] = -1;
			rbopLisse[i] = -1;
			lbopLisse[i] = -1;
			bipLisse[i] = -1;
			lbipLisse[i] = -1;
			dlbipLisse[i] = -1;
		}
		else
		{
			floodLisse[i] /= nbValeurs[i];
			rbopLisse[i] /= nbValeurs[i];
			lbopLisse[i] /= nbValeurs[i];
			bipLisse[i] /= nbValeurs[i];
			lbipLisse[i] /= nbValeurs[i];
			dlbipLisse[i] /= nbValeurs[i];
		}
		
		/*cout << nbValeurs[i] << " : " << densiteLisse[i] << " " << floodLisse[i] << " " << rbopLisse[i] << " " << lbopLisse[i] << " " << bipLisse[i] << " " << lbipLisse[i] << " " << dlbipLisse[i] << endl;*/
		arrondi+=0.05;
	}
	for(i = 0 ; i < densiteLisse.size() ; i++)
	{
		if(floodLisse[i] == -1)
		{
			densiteLisse.erase(densiteLisse.begin()+i);
			floodLisse.erase(floodLisse.begin()+i);
			rbopLisse.erase(rbopLisse.begin()+i);
			lbopLisse.erase(lbopLisse.begin()+i);
			bipLisse.erase(bipLisse.begin()+i);
			lbipLisse.erase(lbipLisse.begin()+i);
			dlbipLisse.erase(dlbipLisse.begin()+i);
			nbValeurs.erase(nbValeurs.begin()+i);
			i--;
		}
	}
	cout << "Nb val\tdensite\tflood\tRBOP\tLBOP\tBIP\tLBIP\tDLBIP" << endl;
	cout.precision(4);
	for(i = 0 ; i < densiteLisse.size() ; i++)
	{
		cout << nbValeurs[i] << "\t" << densiteLisse[i] << "\t" << floodLisse[i] << "\t" << rbopLisse[i] << "\t" << lbopLisse[i] << "\t" << bipLisse[i] << "\t" << lbipLisse[i] << "\t" << dlbipLisse[i] << endl;
		out << densiteLisse[i] << " " << floodLisse[i] << " " << rbopLisse[i] << " " << lbopLisse[i] << " " << bipLisse[i] << " " << lbipLisse[i] << " " << dlbipLisse[i] << endl;
	}
	/*for(unsigned int i = 0 ; i < densite.size() ; i++)
	{
		//cout << "ligne " << i << endl;
		cout << densite[i] << " " << flood[i] << " " << rbop[i] << " " << lbop[i] << " " << bip[i] << " " << lbip[i] << " " << dlbip[i] << endl;
	}*/
	out.close();
	
	return 0;
}


