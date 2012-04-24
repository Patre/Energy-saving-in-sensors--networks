#include <QtGui>
#include "graphView.h"

//! [0]
GraphView::GraphView(QWidget *parent)
    : QWidget(parent)
{
    //INITAILISATION
    haveNodes=false;
    haveGraph=false;
    zoom = 1;


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
    painter.setPen(pen);
    //painter.setBrush(brush);

    if(haveGraph && haveNodes)
    {
        int r=qrand()%127;
        int v=qrand()%127;
        int b=qrand()%127;

        painter.setPen(qRgb(r,v,b));

        painter.setBrush(QBrush(QColor(r,v,b)));
        for(int i=0;i<list_graph.size();i++)
        {
            painter.drawLine(list_graph.at(i).nodeDeb.nodePosition*zoom,list_graph.at(i).nodeFin.nodePosition*zoom);
        }
    }
    painter.setPen(qRgb(0,0,0));
    if(haveNodes)
    {
        for(int i=0;i<nodes.size();i++)
        {
            QPointF inter=nodes.at(i).nodePosition;
            inter.setX(inter.x()-10);
            inter.setY(inter.y()-10);
            QRectF rec(inter,QSizeF(20,20));

            painter.fillRect(rec,QBrush(qRgb(255,255,255)));

            painter.drawArc(rec,0,30*360);
            if(i>9)painter.drawText(nodes.at(i).nodePosition.x()-7,nodes.at(i).nodePosition.y()+5,QVariant(nodes.at(i).node).toString());
            else painter.drawText(nodes.at(i).nodePosition.x()-4,nodes.at(i).nodePosition.y()+5,QVariant(nodes.at(i).node).toString());
        }
    }

}

void GraphView::clearGraph()
{
    haveNodes=false;
    haveGraph=false;
    list_graph.clear();
    nodes.clear();
}
