/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 6. May 21:34:16 2012
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCharger_Graphe;
    QAction *actionClear_Graphe;
    QAction *actionQuitter;
    QAction *actionQui_somme_Nous;
    QAction *actionZoomPlus;
    QAction *actionZoomMoins;
    QAction *actionAfficherGraphe;
    QAction *actionAfficherEtape;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QMenu *menuAide;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QDoubleSpinBox *zoomEdit;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *DockLayout;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *horizontalLayout;
    QPushButton *zoomout;
    QPushButton *zoomin;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QPushButton *charger;
    QPushButton *afficherNodes;
    QPushButton *etapeSuivante;
    QPushButton *graphFinale;
    QPushButton *clearButton;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *verticalSpacer_13;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *verticalSpacer_14;
    QSpacerItem *verticalSpacer_10;
    QSpacerItem *verticalSpacer_11;
    QSpinBox *nbrNodes;
    QSpinBox *rangeEdit;
    QSpacerItem *verticalSpacer_16;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("#dockWidget {\n"
"border: none;\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #a6a6a6, stop: 0.08 #7f7f7f,\n"
"stop: 0.39999 #717171, stop: 0.4 #626262,\n"
"stop: 0.9 #4c4c4c, stop: 1 #333333);\n"
"}\n"
"\n"
"#dockWidget QPushButton {\n"
"color: #333;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"padding: 5px;\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #888);\n"
"min-width: 80px;\n"
"}\n"
"#dockWidget QSpinBox {\n"
"border: 2px solid gray;\n"
"     border-radius: 10px;\n"
"     padding: 0 8px;\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"\n"
"     selection-background-color: darkgray;\n"
"}\n"
"\n"
"#dockWidget QPushButton:hover {\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #bbb);\n"
"}\n"
"\n"
"#dockWidget QPushButton:pressed {\n"
"ba"
                        "ckground: qradialgradient(cx: 0.4, cy: -0.1,\n"
"fx: 0.4, fy: -0.1,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
"}"));
        actionCharger_Graphe = new QAction(MainWindow);
        actionCharger_Graphe->setObjectName(QString::fromUtf8("actionCharger_Graphe"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/ressources/edit_add-256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCharger_Graphe->setIcon(icon);
        actionClear_Graphe = new QAction(MainWindow);
        actionClear_Graphe->setObjectName(QString::fromUtf8("actionClear_Graphe"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/ressources/exit-128.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear_Graphe->setIcon(icon1);
        actionQuitter = new QAction(MainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/ressources/Gnome-Application-Exit-64.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuitter->setIcon(icon2);
        actionQui_somme_Nous = new QAction(MainWindow);
        actionQui_somme_Nous->setObjectName(QString::fromUtf8("actionQui_somme_Nous"));
        actionZoomPlus = new QAction(MainWindow);
        actionZoomPlus->setObjectName(QString::fromUtf8("actionZoomPlus"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/ressources/viewmag+-256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomPlus->setIcon(icon3);
        actionZoomMoins = new QAction(MainWindow);
        actionZoomMoins->setObjectName(QString::fromUtf8("actionZoomMoins"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/ressources/viewmag--256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomMoins->setIcon(icon4);
        actionAfficherGraphe = new QAction(MainWindow);
        actionAfficherGraphe->setObjectName(QString::fromUtf8("actionAfficherGraphe"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/ressources/2rightarrow-256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAfficherGraphe->setIcon(icon5);
        actionAfficherEtape = new QAction(MainWindow);
        actionAfficherEtape->setObjectName(QString::fromUtf8("actionAfficherEtape"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/ressources/1rightarrow-256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAfficherEtape->setIcon(icon6);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        menuAide = new QMenu(menubar);
        menuAide->setObjectName(QString::fromUtf8("menuAide"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setEnabled(true);
        dockWidget->setMinimumSize(QSize(220, 36));
        dockWidget->setStyleSheet(QString::fromUtf8(""));
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
        verticalLayoutWidget = new QWidget(dockWidgetContents);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 204, 401));
        DockLayout = new QVBoxLayout(verticalLayoutWidget);
        DockLayout->setObjectName(QString::fromUtf8("DockLayout"));
        DockLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_7);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_8);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_9);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        zoomout = new QPushButton(verticalLayoutWidget);
        zoomout->setObjectName(QString::fromUtf8("zoomout"));
        zoomout->setMinimumSize(QSize(94, 30));
        zoomout->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(zoomout);

        zoomin = new QPushButton(verticalLayoutWidget);
        zoomin->setObjectName(QString::fromUtf8("zoomin"));
        zoomin->setMinimumSize(QSize(94, 30));
        zoomin->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(zoomin);


        DockLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_2);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_5);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_4);

        charger = new QPushButton(verticalLayoutWidget);
        charger->setObjectName(QString::fromUtf8("charger"));
        charger->setMinimumSize(QSize(94, 30));

        DockLayout->addWidget(charger);

        afficherNodes = new QPushButton(verticalLayoutWidget);
        afficherNodes->setObjectName(QString::fromUtf8("afficherNodes"));
        afficherNodes->setEnabled(false);
        afficherNodes->setMinimumSize(QSize(94, 30));

        DockLayout->addWidget(afficherNodes);

        etapeSuivante = new QPushButton(verticalLayoutWidget);
        etapeSuivante->setObjectName(QString::fromUtf8("etapeSuivante"));
        etapeSuivante->setEnabled(false);
        etapeSuivante->setMinimumSize(QSize(94, 30));

        DockLayout->addWidget(etapeSuivante);

        graphFinale = new QPushButton(verticalLayoutWidget);
        graphFinale->setObjectName(QString::fromUtf8("graphFinale"));
        graphFinale->setEnabled(false);
        graphFinale->setMinimumSize(QSize(94, 30));

        DockLayout->addWidget(graphFinale);

        clearButton = new QPushButton(verticalLayoutWidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setEnabled(true);
        clearButton->setMinimumSize(QSize(94, 30));

        DockLayout->addWidget(clearButton);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_12);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_13);

        verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_15);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_14);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_10);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_11);

        nbrNodes = new QSpinBox(verticalLayoutWidget);
        nbrNodes->setObjectName(QString::fromUtf8("nbrNodes"));
        nbrNodes->setEnabled(false);
        nbrNodes->setMinimumSize(QSize(0, 30));
        nbrNodes->setMaximumSize(QSize(16777215, 16));

        DockLayout->addWidget(nbrNodes);

        rangeEdit = new QSpinBox(verticalLayoutWidget);
        rangeEdit->setObjectName(QString::fromUtf8("rangeEdit"));
        rangeEdit->setEnabled(false);
        rangeEdit->setMinimumSize(QSize(0, 30));
        rangeEdit->setMaximumSize(QSize(16777215, 16));
        rangeEdit->setValue(30);

        DockLayout->addWidget(rangeEdit);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        DockLayout->addItem(verticalSpacer_16);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFichier->menuAction());
        menubar->addAction(menuAide->menuAction());
        menuFichier->addAction(actionCharger_Graphe);
        menuFichier->addAction(actionClear_Graphe);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);
        menuAide->addAction(actionQui_somme_Nous);
        toolBar->addAction(actionCharger_Graphe);
        toolBar->addAction(actionClear_Graphe);
        toolBar->addSeparator();
        toolBar->addAction(actionZoomPlus);
        toolBar->addAction(actionZoomMoins);
        toolBar->addSeparator();
        toolBar->addAction(actionAfficherGraphe);
        toolBar->addAction(actionAfficherEtape);

        retranslateUi(MainWindow);
        QObject::connect(zoomEdit, SIGNAL(valueChanged(double)), MainWindow, SLOT(updateZoom(double)));
        QObject::connect(charger, SIGNAL(clicked()), MainWindow, SLOT(chargerGraph()));
        QObject::connect(etapeSuivante, SIGNAL(clicked()), MainWindow, SLOT(afficherEtape()));
        QObject::connect(graphFinale, SIGNAL(clicked()), MainWindow, SLOT(afficherGraph()));
        QObject::connect(clearButton, SIGNAL(clicked()), MainWindow, SLOT(clearGraph()));
        QObject::connect(zoomin, SIGNAL(clicked()), MainWindow, SLOT(updateZoomPlus()));
        QObject::connect(zoomout, SIGNAL(clicked()), MainWindow, SLOT(updateZoomMoins()));
        QObject::connect(actionCharger_Graphe, SIGNAL(triggered()), MainWindow, SLOT(chargerGraph()));
        QObject::connect(actionClear_Graphe, SIGNAL(triggered()), MainWindow, SLOT(clearGraph()));
        QObject::connect(actionQuitter, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionZoomMoins, SIGNAL(triggered()), MainWindow, SLOT(updateZoomMoins()));
        QObject::connect(actionZoomPlus, SIGNAL(triggered()), MainWindow, SLOT(updateZoomPlus()));
        QObject::connect(actionAfficherGraphe, SIGNAL(triggered()), MainWindow, SLOT(afficherGraph()));
        QObject::connect(actionAfficherEtape, SIGNAL(triggered()), MainWindow, SLOT(afficherEtape()));
        QObject::connect(afficherNodes, SIGNAL(clicked()), MainWindow, SLOT(afficherNodes()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Visualisateur de r\303\251seau", 0, QApplication::UnicodeUTF8));
        actionCharger_Graphe->setText(QApplication::translate("MainWindow", "Charger un Graphe", 0, QApplication::UnicodeUTF8));
        actionClear_Graphe->setText(QApplication::translate("MainWindow", "Effacer le Graphe", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("MainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        actionQui_somme_Nous->setText(QApplication::translate("MainWindow", "Qui somme Nous?", 0, QApplication::UnicodeUTF8));
        actionZoomPlus->setText(QApplication::translate("MainWindow", "zoomPlus", 0, QApplication::UnicodeUTF8));
        actionZoomMoins->setText(QApplication::translate("MainWindow", "ZoomMoins", 0, QApplication::UnicodeUTF8));
        actionAfficherGraphe->setText(QApplication::translate("MainWindow", "afficherGraphe", 0, QApplication::UnicodeUTF8));
        actionAfficherEtape->setText(QApplication::translate("MainWindow", "afficherEtape", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("MainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
        menuAide->setTitle(QApplication::translate("MainWindow", "Aide", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        dockWidget->setWhatsThis(QApplication::translate("MainWindow", "Dock qui permet de charger et verfi\303\251r le Graphe", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        zoomEdit->setPrefix(QString());
        zoomEdit->setSuffix(QApplication::translate("MainWindow", "%", 0, QApplication::UnicodeUTF8));
        zoomout->setText(QApplication::translate("MainWindow", "Zoom-", 0, QApplication::UnicodeUTF8));
        zoomin->setText(QApplication::translate("MainWindow", "Zoom+", 0, QApplication::UnicodeUTF8));
        charger->setText(QApplication::translate("MainWindow", "Charger un graphe", 0, QApplication::UnicodeUTF8));
        afficherNodes->setText(QApplication::translate("MainWindow", "Afficher les n\305\223uds", 0, QApplication::UnicodeUTF8));
        etapeSuivante->setText(QApplication::translate("MainWindow", "Etape suivante", 0, QApplication::UnicodeUTF8));
        graphFinale->setText(QApplication::translate("MainWindow", "Graphe final", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("MainWindow", "Effacer", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
