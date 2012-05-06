#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QScrollArea>

#include "graphView.h"
#include "graphelement.h"


//ADDED Function Prototypes
double distanceEuclidienne(double x1,double y1, double x2, double y2);
double distanceEuclidienne(Element x1,Element x2);

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void chargerGraph();
    void afficherNodes();
    void afficherGrapheInitial();
    void afficherEtape();
    void afficherGraph();
    void clearGraph();
    void calculerDegree();
    void updateImage(QImage image);
    void updateZoom(double x);
    void updateZoomPlus();
    void updateZoomMoins();


private:
    void lireFile(QString filename);
    void createActions();

    Ui::MainWindow  *ui;
    GraphView       *graph_view;
    QList<GraphElement> list_graph;
    QList<int> list_etape;
    QList<GraphElement> list_graph_etape;
    QList<Element>  nodes;
    double degreeMoy;
    double zoom;

    QScrollArea *scrollArea;
    QImage *image;

};

#endif // MAINWINDOW_H
