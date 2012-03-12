#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "graphView.h"
#include "graphelement.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void lireFile(QString filename);

public slots:
    void chargerGraph();
    void afficherNodes();
    void afficherGraph();
    void clearGraph();

private:
    Ui::MainWindow  *ui;
    GraphView       *graph_view;
    QList<GraphElement> list_graph;
    QList<Element>  nodes;
    int zoom;
};

#endif // MAINWINDOW_H
