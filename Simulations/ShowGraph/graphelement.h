#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H
#include <QPointF>

struct Element
{
    int node;
    QPointF nodePosition;
};

class GraphElement
{
public:
    GraphElement(int deb,int fin,qreal debx,qreal deby,qreal finx,qreal finy);
    GraphElement(Element deb,Element fin);
    GraphElement(const  GraphElement  &autre);



    int getIdNodeDebut();
    int getIdNodeFin();
    void debug();
    static void debug(GraphElement ele);
    static void debugElement(Element ele);

    Element nodeDeb;
    Element nodeFin;
};
#endif // GRAPHELEMENT_H
