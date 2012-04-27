#include "topogenerate.h"

TopoGenerate::TopoGenerate()
{
    this->boolCarreau=false;
    this->boolSize=false;
    this->boolIndex=false;
    this->boolfinish=false;
}

void TopoGenerate::setSizeTopo(int width, int heigth)
{
    this->width=width;
    this->heigth=heigth;
    this->boolSize=true;
}

void TopoGenerate::setSizeCarreau(int widthCarreau, int heigthCarreau)
{
    this->widthCarreau=widthCarreau;
    this->heigthCarreau=heigthCarreau;
    this->boolCarreau=true;
}

bool TopoGenerate::calculeIndexs()
{
    if(this->boolCarreau && this->boolSize)
    {
        this->Wi=(qreal)this->width/(qreal) this->widthCarreau;
        He=(qreal)this->heigth/(qreal) this->heigthCarreau;
        this->boolIndex=true;
        return true;
    }
    else return false;
}

bool TopoGenerate::genererNeoudes()
{
    if(this->boolCarreau && this->boolIndex && this->boolSize)
    {
        for(int w=0;w<Wi;w++)
            for(int h=0;h<He;h++)
            {
                carreau[w][h].w= w*this->widthCarreau+qrand()%(this->widthCarreau/2)+(this->widthCarreau/4);
                carreau[w][h].h= h*this->heigthCarreau+qrand()%(this->heigthCarreau/2)+(this->heigthCarreau/4);
            }
        this->boolfinish=true;
        return true;
    }
    else return false;
}

bool TopoGenerate::genererFichier()
{

    if(this->boolfinish)
    {
        int i=0;
        FILE *f;
        f=fopen("topo.txt","w");
        FILE *fxml;
        fxml=fopen("xml.txt","w");

        for(int w=0;w<Wi;w++)
            for(int h=0;h<He;h++)
            {
                fprintf(f,"N: %d %f %f\n", i,carreau[w][h].w,carreau[w][h].h);
                fprintf(fxml,"<node id=\"%d\"  > \n\t<for entity=\"static-pos\" x=\"%f\" y=\"%f\" z=\"0.0\" />\n</node>\n",
                        i,carreau[w][h].w,carreau[w][h].h);

                i++;
            }


        fclose(f);
        fclose(fxml);
        return true;
    }
    else return false;
}

