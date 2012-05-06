#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QtGui>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "graphelement.h"

class GraphView : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    GraphView(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setNodes(QList<Element> nodesPere);
    void setGraph(QList<GraphElement> graph);
    void setGraphEtape(QList<int> dep);
    void clearGraph();
    void setTaille(int widthV, int heightV);
    void setZoom(double zoom);
    void paint();

signals:
    void updateImage(QImage image);


protected:
    void paintEvent(QPaintEvent *event);

private:
    //LES NODEs
    bool haveNodes;
    QList<Element>  nodes;

    //LES liens
    bool haveGraph;
    QList<GraphElement> list_graph;


    //les depart
    bool haveDepart;
    QList<int> depart;

    QList<int> dejaTrai;
    int indexIMG;

    double zoom;

    qreal widthView;
    qreal heightView;



    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;

    //SCAL
};


#endif // GRAPHVIEW_H
