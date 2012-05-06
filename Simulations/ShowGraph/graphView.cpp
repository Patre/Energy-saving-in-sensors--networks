#include "graphView.h"

//! [0]

double distance (GraphElement s)
{
    double dx=s.nodeDeb.nodePosition.x()-s.nodeFin.nodePosition.x();
    double dy=s.nodeDeb.nodePosition.y()-s.nodeFin.nodePosition.y();
    return qSqrt(dx*dx+dy*dy);
}
double distance(Element a, Element b)
{
    double dx=a.nodePosition.x()-b.nodePosition.x();
    double dy=a.nodePosition.y()-b.nodePosition.y();
    return qSqrt(dx*dx+dy*dy);

}


GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
{
    //INITAILISATION
    haveNodes=false;
    haveGraph=false;
    haveDepart=false;
    zoom = 3;
    indexIMG=0;
    widthView=1000;
    heightView=1000;


    shape = Polygon;
    antialiased = false;
    transformed = false;

    setAutoFillBackground(true);
}
void GraphView::setNodes(QList<Element> nodesPere)
{
    nodes=nodesPere;
    haveNodes=true;
}

void GraphView::setGraph(QList<GraphElement> graph)
{
    list_graph=graph;
    haveGraph=true;
}
void GraphView::setGraphEtape(QList<int> dep)
{
    depart.clear();
    depart=dep;
    haveDepart=true;
}

QSize GraphView::minimumSizeHint() const
{
    return QSize(100, 100);
}
QSize GraphView::sizeHint() const
{
    return QSize(400, 200);
}
void GraphView::setZoom(double z)
{
    this->zoom = z;
    widthView = widthView*z;
    heightView = heightView*z;
    for(int i=0;i<nodes.length();i++)
    {
        Element inter=nodes.at(i);
        qreal x=inter.nodePosition.x();
        qreal y=inter.nodePosition.y();
        inter.nodePosition.setX(x*z);
        inter.nodePosition.setY(y*z);
        nodes.replace(i,inter);
    }
}
void GraphView::paintEvent(QPaintEvent * /* event */)
{

}

void GraphView::clearGraph()
{
    haveNodes=false;
    haveGraph=false;
    list_graph.clear();
    depart.clear();
    dejaTrai.clear();
    nodes.clear();
    indexIMG=0;
}

void GraphView::setTaille(int widthV, int heightV)
{
    widthView=widthV;
    heightView=heightV;
}

void GraphView::paint()
{
    QImage *image=new QImage(widthView,heightView,QImage::Format_ARGB32);
    image->fill(qRgb(255,255,255));
    QPainter painter(image);
    QMap<int,int> rayonMax;

    if(haveDepart)
    {
        for(int i=0;i<list_graph.size();i++)
        {
            GraphElement encours =list_graph.at(i);

            if(depart.contains(encours.nodeDeb.node))
            {
                    if(!rayonMax.contains(encours.nodeDeb.node))
                    {
                        rayonMax.insert(encours.nodeDeb.node, encours.nodeFin.node);
                    }
                    else
                    {
                        GraphElement *precE;
                        int prec=rayonMax.value(encours.nodeDeb.node);
                        for(int i=0;i<list_graph.length();i++)
                        {
                            if(list_graph.at(i).nodeDeb.node==encours.nodeDeb.node
                                    && list_graph.at(i).nodeFin.node==prec)  precE=new GraphElement(encours);
                        }

                        if(distance(encours)>distance(*precE))
                            rayonMax.insert(encours.nodeDeb.node,encours.nodeFin.node);
                    }
            }
        }
    }


    if(haveGraph && haveNodes)
    {
        for(int i=0;i<list_graph.size();i++)
        {

            int distAff=(int)distance(list_graph.at(i));

            painter.setPen(qRgb(110, 110, 200));


            Element nodeDeb,nodeFin;
            for(int j=0;j<nodes.length();j++)
            {
                if(nodes.at(j).node==list_graph.at(i).nodeDeb.node) nodeDeb=nodes.at(j);
                if(nodes.at(j).node==list_graph.at(i).nodeFin.node) nodeFin=nodes.at(j);
            }
            int dist=(int)distance(nodeDeb,nodeFin);
            QPointF center((nodeDeb.nodePosition+nodeFin.nodePosition)/2);

            //GraphElement::debug(list_graph.at(i));
            //GraphElement::debugElement(nodeDeb);
            //GraphElement::debugElement(nodeFin);

            painter.drawText(center,QVariant((distAff)).toString());
            painter.drawLine(nodeDeb.nodePosition,nodeFin.nodePosition);


            if(haveDepart && depart.contains(nodeDeb.node) && !dejaTrai.contains(nodeDeb.node)
                    && rayonMax.value(nodeDeb.node)== nodeFin.node)
            {
                painter.setPen(qRgb(224, 176, 255));
                painter.drawEllipse(nodeDeb.nodePosition,dist,dist);

            }
        }
    }

    //painter.setPen(qRgb(255,0,0));


    if(haveNodes)
    {
        for(int i=0;i<nodes.size();i++)
        {
            QPointF inter=nodes.at(i).nodePosition;
            inter.setX(inter.x()-12);
            inter.setY(inter.y()-12);
            QRectF rec(inter,QSizeF(24,24));



            QRadialGradient radialGradient( inter, 70);
            radialGradient.setColorAt(0.0, Qt::white);
            radialGradient.setColorAt(0.3, Qt::gray);
            radialGradient.setColorAt(1.0, Qt::black);


            painter.setBrush(radialGradient);
            painter.drawEllipse(rec);

            painter.setPen(qRgb(255,255,255));
            painter.drawText(rec,Qt::AlignCenter, QVariant(nodes.at(i).node).toString());

        }
    }
    dejaTrai.append(depart);
    indexIMG++;//*/

    emit updateImage(*image);
}
