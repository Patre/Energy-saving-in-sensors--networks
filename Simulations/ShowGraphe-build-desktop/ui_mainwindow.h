/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Mar 12 21:52:07 2012
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
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QPushButton *charger;
    QPushButton *afficherNodes;
    QPushButton *etapeSuivante;
    QPushButton *graphFinale;
    QPushButton *clearButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(100, 100));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        charger = new QPushButton(dockWidgetContents);
        charger->setObjectName(QString::fromUtf8("charger"));
        charger->setGeometry(QRect(0, 0, 101, 31));
        afficherNodes = new QPushButton(dockWidgetContents);
        afficherNodes->setObjectName(QString::fromUtf8("afficherNodes"));
        afficherNodes->setEnabled(false);
        afficherNodes->setGeometry(QRect(0, 40, 101, 31));
        etapeSuivante = new QPushButton(dockWidgetContents);
        etapeSuivante->setObjectName(QString::fromUtf8("etapeSuivante"));
        etapeSuivante->setEnabled(false);
        etapeSuivante->setGeometry(QRect(0, 80, 101, 31));
        graphFinale = new QPushButton(dockWidgetContents);
        graphFinale->setObjectName(QString::fromUtf8("graphFinale"));
        graphFinale->setEnabled(false);
        graphFinale->setGeometry(QRect(0, 120, 101, 31));
        clearButton = new QPushButton(dockWidgetContents);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setEnabled(false);
        clearButton->setGeometry(QRect(0, 170, 101, 31));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        retranslateUi(MainWindow);
        QObject::connect(charger, SIGNAL(clicked()), MainWindow, SLOT(chargerGraph()));
        QObject::connect(afficherNodes, SIGNAL(clicked()), MainWindow, SLOT(afficherNodes()));
        QObject::connect(graphFinale, SIGNAL(clicked()), MainWindow, SLOT(afficherGraph()));
        QObject::connect(clearButton, SIGNAL(clicked()), MainWindow, SLOT(clearGraph()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        charger->setText(QApplication::translate("MainWindow", "Charger Graph", 0, QApplication::UnicodeUTF8));
        afficherNodes->setText(QApplication::translate("MainWindow", "Afficher Nodes", 0, QApplication::UnicodeUTF8));
        etapeSuivante->setText(QApplication::translate("MainWindow", "Etape Suivante", 0, QApplication::UnicodeUTF8));
        graphFinale->setText(QApplication::translate("MainWindow", "Graph Finale", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
