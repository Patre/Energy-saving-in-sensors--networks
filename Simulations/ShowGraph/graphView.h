#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

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

    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    void setZoom(double zoom);

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
    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;
};


#endif // GRAPHVIEW_H
