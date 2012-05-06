#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include  <QLabel>






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //INITALISATION
    ui->zoomEdit->setVisible(false);
    //ui->afficherNodes->setVisible(false);
    graph_view=new GraphView();
    zoom = 1;
    image= new QImage();

    scrollArea = new QScrollArea;

    createActions();

    //AFFICHAGE
    setCentralWidget(scrollArea);
}
void MainWindow::updateImage(QImage image)
{
    int ver=scrollArea->verticalScrollBar()->value();
    int hor=scrollArea->horizontalScrollBar()->value();

    QLabel *imageLabel = new QLabel;
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scrollArea->setWidget(imageLabel);
    scrollArea->verticalScrollBar()->setValue(ver);
    scrollArea->horizontalScrollBar()->setValue(hor);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculerDegree()
{
    double range=ui->rangeEdit->value()*zoom;
    int degree=0;
    for(int i=0;i<nodes.size();i++)
        for(int j=0;j<nodes.size();j++)
            if(i!=j && distanceEuclidienne(nodes.at(i),nodes.at(j))<range)
                degree++;
    degreeMoy = (double) degree / (double) nodes.size();
}

void MainWindow::lireFile(QString filename)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(0,"fucked open","impossible d'ouvrir "+filename);
        return;
    }
    while (!file.atEnd()){
        QByteArray line = file.readLine();
        QStringList lst=QVariant(line).toString().split(" ");
        if(lst.at(0)=="N:")
        {
            Element ele;
            ele.node = QVariant(lst.at(1)).toInt();
            ele.nodePosition.setX(QVariant(lst.at(2)).toReal());
            ele.nodePosition.setY(QVariant(lst.at(3)).toReal());
            nodes.append(ele);
        }
        else if(lst.at(0)=="G:")
        {

            int deb = QVariant(lst.at(1)).toInt();
            int fin = QVariant(lst.at(2)).toInt();

            //RECUPERER LES ELMENT DANS NODES
            Element ndeb,nfin;
            for(int i=0;i<nodes.size();i++)
            {
                if(nodes.at(i).node==deb)
                {
                    ndeb.node=nodes.at(i).node;
                    ndeb.nodePosition=nodes.at(i).nodePosition;
                }
                if(nodes.at(i).node==fin)
                {
                    nfin.node=nodes.at(i).node;
                    nfin.nodePosition=nodes.at(i).nodePosition;
                }
            }

            bool test=true;
            GraphElement x(ndeb,nfin);
            //x.debug();
            for(int i=0;i<list_graph.length();i++)
                if(list_graph.at(i).nodeDeb.node==ndeb.node &&
                        list_graph.at(i).nodeFin.node==nfin.node) test=false;
            if(test) list_graph.append(x);
        }
    }

    QPointF max(0,0);
    QPointF min(99999999,99999999);
    for(int i=0;i<nodes.length();i++)
    {
        if(nodes.at(i).nodePosition.x()>max.x())    max.setX(nodes.at(i).nodePosition.x());
        if(nodes.at(i).nodePosition.y()>max.y())    max.setY(nodes.at(i).nodePosition.y());
        if(nodes.at(i).nodePosition.x()<min.x())    min.setX(nodes.at(i).nodePosition.x());
        if(nodes.at(i).nodePosition.y()<min.y())    min.setY(nodes.at(i).nodePosition.y());
    }

    QString beta="MAX "+QVariant(max.x()).toString()+" "+QVariant(max.y()).toString()
            +"\nMIN "+QVariant(min.x()).toString()+" "+QVariant(min.y()).toString();
    //QMessageBox::information(0,"INFO",beta);

    if(min.x()>100)
        for(int i=0;i<nodes.length();i++)
        {
            Element inter= nodes.at(i);
            inter.nodePosition.setX(inter.nodePosition.x()-min.x()+50);
            nodes.replace(i,inter);
        }

    if(min.y()>100)
        for(int i=0;i<nodes.length();i++)
        {
            Element inter= nodes.at(i);
            inter.nodePosition.setY(inter.nodePosition.y()-min.y()+50);
            nodes.replace(i,inter);
        }//*/


    graph_view->setTaille(max.x()-min.x()+20, max.y()-min.y()+20);



    afficherNodes();
    //affichage des nodes
    //for(int i=0;i<nodes.size();i++) GraphElement::debugElement(nodes.at(i));

    //affichages des connecxion
    //for(int i=0;i<list_graph.size();i++) GraphElement::debug(list_graph.at(i));
}



void MainWindow::updateZoom(double x)
{
    //QMessageBox::information(0,"fuck",QVariant(x).toString());
    ui->zoomEdit->setValue(100);
    graph_view->setZoom(x/100.0);
    graph_view->paint();
}

void MainWindow::chargerGraph()
{
    QString file=QFileDialog::getOpenFileName(this,"Ouvrir un fichier de graph");
    lireFile(file);
    ui->nbrNodes->setValue(nodes.length());
    ui->charger->setEnabled(false);
    ui->afficherNodes->setEnabled(true);
    ui->graphFinale->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

void MainWindow::afficherGrapheInitial()
{
    QList<GraphElement> list_initial;
    for(int i=0;i<nodes.length();i++)
        for(int j=0;j<nodes.length();j++)
            if(distanceEuclidienne(nodes.at(i),nodes.at(j))< ui->rangeEdit->value() && i != j)
            {
                GraphElement *inter= new GraphElement(nodes.at(i),nodes.at(j));
                list_initial.append(*inter);
            }


    graph_view->setGraph(list_initial);
    graph_view->paint();
}

void MainWindow::afficherNodes()
{
    graph_view->setNodes(nodes);
    if(!nodes.isEmpty())
    {
        list_etape.append(list_graph.at(0).nodeDeb.node);
        ui->etapeSuivante->setEnabled(true);
    }
    ui->zoomEdit->setEnabled(true);
    graph_view->paint();
}
void MainWindow::afficherEtape()
{
    QList<int> tmp;
    for(int i=0;i<list_graph.count();i++)
        if(list_etape.contains(list_graph.at(i).nodeDeb.node))
        {
            tmp.append(list_graph.at(i).nodeFin.node);
            list_graph_etape.append(list_graph.at(i));
        }

    graph_view->setGraph(list_graph_etape);
    graph_view->setGraphEtape(list_etape);
    graph_view->paint();

    list_etape.clear();
    list_etape=tmp;

}

void MainWindow::afficherGraph()
{
    graph_view->setGraph(list_graph);
    graph_view->paint();
}

void MainWindow::clearGraph()
{
    ui->charger->setEnabled(true);
    ui->afficherNodes->setEnabled(false);
    ui->graphFinale->setEnabled(false);
    ui->etapeSuivante->setEnabled(false);
    ui->zoomEdit->setEnabled(false);
    list_graph.clear();
    nodes.clear();
    list_etape.clear();
    list_graph_etape.clear();
    graph_view->clearGraph();
    graph_view->repaint();
    updateImage(QImage());
}


void MainWindow::updateZoomPlus()
{
    double x=ui->zoomEdit->value()+10;
    updateZoom(x);
}
void MainWindow::updateZoomMoins()
{
    double x=ui->zoomEdit->value()-10;
    updateZoom(x);
}


void MainWindow::createActions()
{
    connect(graph_view,SIGNAL(updateImage(QImage)),this,SLOT(updateImage(QImage)));
}

//ADDED Function
double distanceEuclidienne(double x1,double y1, double x2, double y2)
{
    return qSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

double distanceEuclidienne(Element x1,Element x2)
{
    return distanceEuclidienne(x1.nodePosition.x(),x1.nodePosition.y(),x2.nodePosition.x(),x2.nodePosition.y());
}


