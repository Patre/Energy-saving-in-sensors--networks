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
    graph_view=new GraphView();
    ui->dockWidget_3->setVisible(false);
    zoom = 3;


    //


    //AFFICHAGE
     setCentralWidget(graph_view );
}
void MainWindow::genererTopologie()
{
    TopoGenerate *ss=new TopoGenerate();
    ss->setSizeTopo(ui->widthTopo->value(),ui->heigthTopo->value());
    ss->setSizeCarreau(ui->widthCar->value(),ui->heigthCar->value());
    if(!ss->calculeIndexs())
        QMessageBox::information(0,"ll","GEneration des index");
    else if(!ss->genererNeoudes())
        QMessageBox::information(0,"fff","GEneration des noueds");
    else if(!ss->genererFichier())
        QMessageBox::information(0,"ss","generation de ficjoier");//*/
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

    zoom = ui->zoomEdit->value();
    graph_view->setZoom(zoom);
    ui->zoomEdit->setEnabled(false);

    while (!file.atEnd()){
        QByteArray line = file.readLine();
        QStringList lst=QVariant(line).toString().split(" ");
        if(lst.at(0)=="N:")
        {
            Element ele;
            ele.node = QVariant(lst.at(1)).toInt();
            ele.nodePosition.setX(QVariant(lst.at(2)).toReal()*zoom);
            ele.nodePosition.setY(QVariant(lst.at(3)).toReal()*zoom);
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

    //affichage des nodes
    //for(int i=0;i<nodes.size();i++) GraphElement::debugElement(nodes.at(i));

    //affichages des connecxion
    //for(int i=0;i<list_graph.size();i++) GraphElement::debug(list_graph.at(i));



}




void MainWindow::chargerGraph()
{
    //zoom=ui->zoomEdit->value();
    QString file=QFileDialog::getOpenFileName(this,"Ouvrir un fichier de graph");
    lireFile(file);
    ui->nbrNodes->setValue(nodes.length());
    //QMessageBox::information(0,"fucked open","Degree Moyen est de "+QVariant(degreeMoy).toString());
    ui->charger->setEnabled(false);
    ui->afficherNodes->setEnabled(true);
    ui->graphFinale->setEnabled(true);
    ui->clearButton->setEnabled(true);
    //ui->etapeSuivante->setEnabled(true);
}

void MainWindow::afficherNodes()
{
    graph_view->setNodes(nodes);
    if(!nodes.isEmpty())
    {
        list_etape.append(list_graph.at(0).nodeDeb.node);
        ui->etapeSuivante->setEnabled(true);
    }
    graph_view->repaint();
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
    graph_view->repaint();

    list_etape.clear();
    list_etape=tmp;

}

void MainWindow::afficherGraph()
{
    graph_view->setGraph(list_graph);
    graph_view->repaint();
}

void MainWindow::clearGraph()
{
    ui->charger->setEnabled(true);
    ui->afficherNodes->setEnabled(false);
    ui->graphFinale->setEnabled(false);
    ui->etapeSuivante->setEnabled(false);
    ui->zoomEdit->setEnabled(true);
    list_graph.clear();
    nodes.clear();
    list_etape.clear();
    list_graph_etape.clear();
    graph_view->clearGraph();
    graph_view->repaint();
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
