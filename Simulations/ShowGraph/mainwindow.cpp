#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //INITALISATION
    graph_view=new GraphView();

    zoom = 5;
    //AFFICHAGE
    setCentralWidget(graph_view);

}

MainWindow::~MainWindow()
{
    delete ui;
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

            GraphElement x(ndeb,nfin);
            //x.debug();
            list_graph.append(x);
        }
    }

    //affichage des nodes
    //for(int i=0;i<nodes.size();i++) GraphElement::debugElement(nodes.at(i));

    //affichages des connecxion
    //for(int i=0;i<list_graph.size();i++) GraphElement::debug(list_graph.at(i));

}




void MainWindow::chargerGraph()
{
    QString file=QFileDialog::getOpenFileName(this,"Ouvrir un fichier de graph");
    lireFile(file);
    ui->charger->setEnabled(false);
    ui->afficherNodes->setEnabled(true);
    ui->graphFinale->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

void MainWindow::afficherNodes()
{
    graph_view->setNodes(nodes);
    graph_view->repaint();
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
    graph_view->clearGraph();
    graph_view->repaint();
}
