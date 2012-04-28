#include <QtGui>
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


    shape = Polygon;
    antialiased = false;
    transformed = false;
    pixmap.load(":/images/qt-logo.png");

    setBackgroundRole(QPalette::Base);
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
void GraphView::setShape(Shape shape)
{
    this->shape = shape;
    update();
}
void GraphView::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}
void GraphView::setZoom(double z)
{
    this->zoom = z;
}

void GraphView::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}
void GraphView::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}
void GraphView::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}


void GraphView::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QMap<int,int> rayonMax;

    if(haveDepart)
    {
        for(int i=0;i<list_graph.size();i++)
        {
            GraphElement encours =list_graph.at(i);

            if(depart.contains(encours.nodeDeb.node))
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


    if(haveGraph && haveNodes)
    {
        for(int i=0;i<list_graph.size();i++)
        {

            int dist=(int)distance(list_graph.at(i));
            QPointF center((list_graph.at(i).nodeDeb.nodePosition+list_graph.at(i).nodeFin.nodePosition)/2);

            painter.setPen(qRgb(110, 110, 200));


            painter.drawText(center,QVariant((int)(dist/zoom)).toString());
            painter.drawLine(list_graph.at(i).nodeDeb.nodePosition,list_graph.at(i).nodeFin.nodePosition);

            if(haveDepart && depart.contains(list_graph.at(i).nodeDeb.node) && !dejaTrai.contains(list_graph.at(i).nodeDeb.node)
                    && rayonMax.value(list_graph.at(i).nodeDeb.node)== list_graph.at(i).nodeFin.node)
            {
                painter.setPen(qRgba(224, 176, 255,75));
                painter.drawEllipse(list_graph.at(i).nodeDeb.nodePosition,dist,dist);
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
}

void GraphView::clearGraph()
{
    haveNodes=false;
    haveGraph=false;
    list_graph.clear();
    depart.clear();
    dejaTrai.clear();
    nodes.clear();
}
