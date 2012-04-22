#ifndef TOPOGENERATE_H
#define TOPOGENERATE_H
#include <QtCore>
typedef struct _element
{
    double h;
    double w;
}element;


class TopoGenerate
{
public:
    TopoGenerate();
    void setSizeTopo(int width, int heigth);
    void setSizeCarreau(int width, int heigth);
    bool calculeIndexs();
    bool genererNeoudes();
    bool genererFichier();


private:
    bool boolSize;
    int width;
    int heigth;

    bool boolCarreau;
    int widthCarreau;
    int heigthCarreau;


    bool boolIndex;
    int Wi;
    int He;

    bool boolfinish;
    element carreau[100][100];
};

#endif // TOPOGENERATE_H
