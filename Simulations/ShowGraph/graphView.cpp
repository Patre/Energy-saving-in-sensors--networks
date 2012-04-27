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

    if(haveGraph && haveNodes)
    {
        for(int i=0;i<list_graph.size();i++)
        {
            QPointF center(list_graph.at(i).nodeDeb.nodePosition.x()+list_graph.at(i).nodeFin.nodePosition.x(),
                           list_graph.at(i).nodeDeb.nodePosition.y()+list_graph.at(i).nodeFin.nodePosition.y());
            center.setX(center.x()/(qreal)2);
            center.setY(center.y()/(qreal)2);


            QMessageBox::information(0,"DEBUG de GARPH ELEMENT",QVariant(center).toString());

            painter.drawText(center+list_graph.at(i).nodeDeb.nodePosition,"LOL");
            painter.drawLine(list_graph.at(i).nodeDeb.nodePosition*zoom,list_graph.at(i).nodeFin.nodePosition*zoom);

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
}

void GraphView::clearGraph()
{
    haveNodes=false;
    haveGraph=false;
    list_graph.clear();
    nodes.clear();
}
