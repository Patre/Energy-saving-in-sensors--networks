/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 1. May 08:10:55 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCharger_Graph;
    QAction *actionClear_Graph;
    QAction *actionQuitter;
    QWidget *centralWidget;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_3;
    QSpinBox *widthTopo;
    QSpinBox *heigthTopo;
    QSpinBox *widthCar;
    QSpinBox *heigthCar;
    QPushButton *pushButton;
    QSpinBox *rangeEdit;
    QMenuBar *menuBar;
    QMenu *menuFichier;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QPushButton *charger;
    QPushButton *afficherNodes;
    QPushButton *etapeSuivante;
    QPushButton *graphFinale;
    QPushButton *clearButton;
    QDoubleSpinBox *zoomEdit;
    QSpinBox *nbrNodes;
    QSpinBox *rayonMax;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(744, 450);
        actionCharger_Graph = new QAction(MainWindow);
        actionCharger_Graph->setObjectName(QString::fromUtf8("actionCharger_Graph"));
        actionClear_Graph = new QAction(MainWindow);
        actionClear_Graph->setObjectName(QString::fromUtf8("actionClear_Graph"));
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        dockWidget_3 = new QDockWidget(centralWidget);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        dockWidget_3->setGeometry(QRect(540, 0, 100, 341));
        dockWidget_3->setMinimumSize(QSize(100, 41));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        widthTopo = new QSpinBox(dockWidgetContents_3);
        widthTopo->setObjectName(QString::fromUtf8("widthTopo"));
        widthTopo->setGeometry(QRect(0, 10, 101, 22));
        widthTopo->setMinimum(100);
        widthTopo->setMaximum(9999);
        heigthTopo = new QSpinBox(dockWidgetContents_3);
        heigthTopo->setObjectName(QString::fromUtf8("heigthTopo"));
        heigthTopo->setGeometry(QRect(0, 50, 101, 22));
        heigthTopo->setMinimum(100);
        heigthTopo->setMaximum(9999);
        widthCar = new QSpinBox(dockWidgetContents_3);
        widthCar->setObjectName(QString::fromUtf8("widthCar"));
        widthCar->setGeometry(QRect(0, 90, 101, 22));
        widthCar->setMinimum(10);
        widthCar->setMaximum(999);
        heigthCar = new QSpinBox(dockWidgetContents_3);
        heigthCar->setObjectName(QString::fromUtf8("heigthCar"));
        heigthCar->setGeometry(QRect(0, 130, 101, 22));
        heigthCar->setMinimum(10);
        heigthCar->setMaximum(999);
        pushButton = new QPushButton(dockWidgetContents_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 210, 101, 23));
        rangeEdit = new QSpinBox(dockWidgetContents_3);
        rangeEdit->setObjectName(QString::fromUtf8("rangeEdit"));
        rangeEdit->setGeometry(QRect(0, 170, 101, 22));
        rangeEdit->setMinimum(10);
        rangeEdit->setMaximum(999);
        rangeEdit->setValue(30);
        dockWidget_3->setWidget(dockWidgetContents_3);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 744, 25));
        menuFichier = new QMenu(menuBar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(180, 100));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        charger = new QPushButton(dockWidgetContents);
        charger->setObjectName(QString::fromUtf8("charger"));
        charger->setGeometry(QRect(0, 40, 171, 31));
        afficherNodes = new QPushButton(dockWidgetContents);
        afficherNodes->setObjectName(QString::fromUtf8("afficherNodes"));
        afficherNodes->setEnabled(false);
        afficherNodes->setGeometry(QRect(0, 80, 171, 31));
        etapeSuivante = new QPushButton(dockWidgetContents);
        etapeSuivante->setObjectName(QString::fromUtf8("etapeSuivante"));
        etapeSuivante->setEnabled(false);
        etapeSuivante->setGeometry(QRect(0, 120, 171, 31));
        graphFinale = new QPushButton(dockWidgetContents);
        graphFinale->setObjectName(QString::fromUtf8("graphFinale"));
        graphFinale->setEnabled(false);
        graphFinale->setGeometry(QRect(0, 160, 171, 31));
        clearButton = new QPushButton(dockWidgetContents);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setEnabled(false);
        clearButton->setGeometry(QRect(0, 210, 171, 31));
        zoomEdit = new QDoubleSpinBox(dockWidgetContents);
        zoomEdit->setObjectName(QString::fromUtf8("zoomEdit"));
        zoomEdit->setGeometry(QRect(110, 0, 61, 22));
        zoomEdit->setValue(3);
        nbrNodes = new QSpinBox(dockWidgetContents);
        nbrNodes->setObjectName(QString::fromUtf8("nbrNodes"));
        nbrNodes->setEnabled(false);
        nbrNodes->setGeometry(QRect(120, 260, 51, 22));
        nbrNodes->setMinimum(0);
        nbrNodes->setMaximum(999);
        nbrNodes->setValue(0);
        rayonMax = new QSpinBox(dockWidgetContents);
        rayonMax->setObjectName(QString::fromUtf8("rayonMax"));
        rayonMax->setEnabled(false);
        rayonMax->setGeometry(QRect(120, 300, 51, 22));
        rayonMax->setMinimum(10);
        rayonMax->setMaximum(999);
        rayonMax->setValue(30);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 91, 17));
        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 260, 91, 17));
        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 300, 91, 17));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        QWidget::setTabOrder(charger, afficherNodes);
        QWidget::setTabOrder(afficherNodes, etapeSuivante);
        QWidget::setTabOrder(etapeSuivante, graphFinale);
        QWidget::setTabOrder(graphFinale, clearButton);
        QWidget::setTabOrder(clearButton, zoomEdit);
        QWidget::setTabOrder(zoomEdit, widthTopo);
        QWidget::setTabOrder(widthTopo, heigthTopo);
        QWidget::setTabOrder(heigthTopo, widthCar);
        QWidget::setTabOrder(widthCar, heigthCar);
        QWidget::setTabOrder(heigthCar, pushButton);
        QWidget::setTabOrder(pushButton, rangeEdit);

        menuBar->addAction(menuFichier->menuAction());
        menuFichier->addAction(actionCharger_Graph);
        menuFichier->addAction(actionClear_Graph);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);

        retranslateUi(MainWindow);
        QObject::connect(charger, SIGNAL(clicked()), MainWindow, SLOT(chargerGraph()));
        QObject::connect(afficherNodes, SIGNAL(clicked()), MainWindow, SLOT(afficherNodes()));
        QObject::connect(graphFinale, SIGNAL(clicked()), MainWindow, SLOT(afficherGraph()));
        QObject::connect(clearButton, SIGNAL(clicked()), MainWindow, SLOT(clearGraph()));
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindow, SLOT(genererTopologie()));
        QObject::connect(etapeSuivante, SIGNAL(clicked()), MainWindow, SLOT(afficherEtape()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionCharger_Graph->setText(QApplication::translate("MainWindow", "Charger Graph", 0, QApplication::UnicodeUTF8));
        actionClear_Graph->setText(QApplication::translate("MainWindow", "Clear Graph", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("MainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Generer Fichier", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
        charger->setText(QApplication::translate("MainWindow", "Charger Graph", 0, QApplication::UnicodeUTF8));
        afficherNodes->setText(QApplication::translate("MainWindow", "Afficher Nodes", 0, QApplication::UnicodeUTF8));
        etapeSuivante->setText(QApplication::translate("MainWindow", "Etape Suivante", 0, QApplication::UnicodeUTF8));
        graphFinale->setText(QApplication::translate("MainWindow", "Graph Finale", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Zoom", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Nbr Nodes", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Rayon Max", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
