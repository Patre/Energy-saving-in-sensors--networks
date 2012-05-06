/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 6. May 15:55:29 2012
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCharger_Graphe;
    QAction *actionClear_Graphe;
    QAction *actionQuitter;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QDoubleSpinBox *zoomEdit;
    QSpinBox *rangeEdit;
    QSpinBox *nbrNodes;
    QPushButton *charger;
    QPushButton *afficherNodes;
    QPushButton *etapeSuivante;
    QPushButton *graphFinale;
    QPushButton *clearButton;
    QPushButton *zoomin;
    QPushButton *zoomout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(800, 600);
        actionCharger_Graphe = new QAction(MainWindow);
        actionCharger_Graphe->setObjectName(QString::fromUtf8("actionCharger_Graphe"));
        actionClear_Graphe = new QAction(MainWindow);
        actionClear_Graphe->setObjectName(QString::fromUtf8("actionClear_Graphe"));
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setEnabled(true);
        dockWidget->setMinimumSize(QSize(150, 36));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        zoomEdit = new QDoubleSpinBox(dockWidgetContents);
        zoomEdit->setObjectName(QString::fromUtf8("zoomEdit"));
        zoomEdit->setEnabled(false);
        zoomEdit->setGeometry(QRect(40, 470, 61, 22));
        zoomEdit->setMinimum(50);
        zoomEdit->setMaximum(200);
        zoomEdit->setSingleStep(10);
        zoomEdit->setValue(100);
        rangeEdit = new QSpinBox(dockWidgetContents);
        rangeEdit->setObjectName(QString::fromUtf8("rangeEdit"));
        rangeEdit->setEnabled(false);
        rangeEdit->setGeometry(QRect(30, 310, 61, 22));
        rangeEdit->setValue(30);
        nbrNodes = new QSpinBox(dockWidgetContents);
        nbrNodes->setObjectName(QString::fromUtf8("nbrNodes"));
        nbrNodes->setEnabled(false);
        nbrNodes->setGeometry(QRect(30, 280, 61, 22));
        charger = new QPushButton(dockWidgetContents);
        charger->setObjectName(QString::fromUtf8("charger"));
        charger->setGeometry(QRect(10, 80, 101, 23));
        afficherNodes = new QPushButton(dockWidgetContents);
        afficherNodes->setObjectName(QString::fromUtf8("afficherNodes"));
        afficherNodes->setEnabled(false);
        afficherNodes->setGeometry(QRect(10, 110, 101, 23));
        etapeSuivante = new QPushButton(dockWidgetContents);
        etapeSuivante->setObjectName(QString::fromUtf8("etapeSuivante"));
        etapeSuivante->setEnabled(false);
        etapeSuivante->setGeometry(QRect(10, 140, 101, 23));
        graphFinale = new QPushButton(dockWidgetContents);
        graphFinale->setObjectName(QString::fromUtf8("graphFinale"));
        graphFinale->setEnabled(false);
        graphFinale->setGeometry(QRect(10, 170, 101, 23));
        clearButton = new QPushButton(dockWidgetContents);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setEnabled(true);
        clearButton->setGeometry(QRect(10, 200, 101, 23));
        zoomin = new QPushButton(dockWidgetContents);
        zoomin->setObjectName(QString::fromUtf8("zoomin"));
        zoomin->setGeometry(QRect(10, 50, 41, 23));
        zoomin->setStyleSheet(QString::fromUtf8("QPushButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(127, 127, 127, 255), stop:1 rgba(255, 255, 255, 255)); border: none; }"));
        zoomout = new QPushButton(dockWidgetContents);
        zoomout->setObjectName(QString::fromUtf8("zoomout"));
        zoomout->setGeometry(QRect(70, 50, 41, 23));
        zoomout->setStyleSheet(QString::fromUtf8(""));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menubar->addAction(menuFichier->menuAction());
        menuFichier->addAction(actionCharger_Graphe);
        menuFichier->addAction(actionClear_Graphe);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);

        retranslateUi(MainWindow);
        QObject::connect(zoomEdit, SIGNAL(valueChanged(double)), MainWindow, SLOT(updateZoom(double)));
        QObject::connect(charger, SIGNAL(clicked()), MainWindow, SLOT(chargerGraph()));
        QObject::connect(afficherNodes, SIGNAL(clicked()), MainWindow, SLOT(afficherNodes()));
        QObject::connect(etapeSuivante, SIGNAL(clicked()), MainWindow, SLOT(afficherEtape()));
        QObject::connect(graphFinale, SIGNAL(clicked()), MainWindow, SLOT(afficherGraph()));
        QObject::connect(clearButton, SIGNAL(clicked()), MainWindow, SLOT(clearGraph()));
        QObject::connect(zoomin, SIGNAL(clicked()), MainWindow, SLOT(updateZoomPlus()));
        QObject::connect(zoomout, SIGNAL(clicked()), MainWindow, SLOT(updateZoomMoins()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionCharger_Graphe->setText(QApplication::translate("MainWindow", "Charger Graphe", 0, QApplication::UnicodeUTF8));
        actionClear_Graphe->setText(QApplication::translate("MainWindow", "Clear Graphe", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("MainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
        zoomEdit->setPrefix(QString());
        zoomEdit->setSuffix(QApplication::translate("MainWindow", "%", 0, QApplication::UnicodeUTF8));
        charger->setText(QApplication::translate("MainWindow", "Charger Graphe", 0, QApplication::UnicodeUTF8));
        afficherNodes->setText(QApplication::translate("MainWindow", "Afficher Les Nodes ", 0, QApplication::UnicodeUTF8));
        etapeSuivante->setText(QApplication::translate("MainWindow", "Etape Suivante", 0, QApplication::UnicodeUTF8));
        graphFinale->setText(QApplication::translate("MainWindow", "Graphe Finale", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("MainWindow", "Clear Graphe", 0, QApplication::UnicodeUTF8));
        zoomin->setText(QApplication::translate("MainWindow", "zoom+", 0, QApplication::UnicodeUTF8));
        zoomout->setText(QApplication::translate("MainWindow", "zoom-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
