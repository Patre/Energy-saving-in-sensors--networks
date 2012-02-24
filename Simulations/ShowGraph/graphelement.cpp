#include "graphelement.h"
#include <QMessageBox>
#include <QVariant>

GraphElement::GraphElement(int deb,int fin,qreal debx,qreal deby,qreal finx,qreal finy)
{
    nodeDeb.node=deb;
    nodeFin.node=fin;
    nodeDeb.nodePosition = QPointF(debx,deby);
    nodeFin.nodePosition = QPointF(finx,finy);
}

GraphElement::GraphElement(Element deb, Element fin)
{
    nodeDeb.node=deb.node;
    nodeFin.node=fin.node;
    nodeDeb.nodePosition = deb.nodePosition;
    nodeFin.nodePosition = fin.nodePosition;
}

void GraphElement::debug()
{
    QMessageBox::information(0,"DEBUG de GARPH ELEMENT","DEBUT ELEMENT "+QVariant(nodeDeb.node).toString()+
                             "\nAVEC la postion ("+QVariant(nodeDeb.nodePosition.x()).toString()+
                             ","+QVariant(nodeDeb.nodePosition.y()).toString()+
                             ")\n pour\n"
                             "FIN ELEMENT "+QVariant(nodeFin.node).toString()+
                             "\nAVEC la postion ("+QVariant(nodeFin.nodePosition.x()).toString()+
                             ","+QVariant(nodeFin.nodePosition.y()).toString()+
                             ")\n pour\n");

}

void GraphElement::debugElement(Element ele)
{
    QMessageBox::information(0,"DEBUG d'un ELEMENT","ID: "+QVariant(ele.node).toString()+"\nX: "
                             +QVariant(ele.nodePosition.x()).toString()+"\nY: "
                             +QVariant(ele.nodePosition.y()).toString()+"\n");//*/
}

void GraphElement::debug(GraphElement ele)
{
    QString src=QVariant(ele.getIdNodeDebut()).toString();
    QString des=QVariant(ele.getIdNodeFin()).toString();
    QMessageBox::information(0,"DEBUG d'un GRAPHELEMENT","SOURCE : "+src+" \n DESTINATION:"+des);
}
int GraphElement::getIdNodeDebut()
{
    return nodeDeb.node;
}
int GraphElement::getIdNodeFin()
{
    return nodeFin.node;
}
