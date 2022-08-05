/********************************************************************************
** Form generated from reading UI file 'XlabAppMainWindowNIBODP.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef XLABAPPMAINWINDOWNIBODP_H
#define XLABAPPMAINWINDOWNIBODP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_XlabAppMainWindowClass
{
public:
    QAction *openDICOM;
    QAction *openNII;
    QAction *showCross;
    QAction *exitXlabApp;
    QAction *actionc;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_1;
    QVTKWidget *toprightvtk;
    QVTKWidget *bottomrightvtk;
    QVTKWidget *topleftvtk;
    QVTKWidget *bottomleftvtk;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QTreeView *fileNameTree;
    QPushButton *viewButton;
    QPushButton *deleteDataButton;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XlabAppMainWindowClass)
    {
        if (XlabAppMainWindowClass->objectName().isEmpty())
            XlabAppMainWindowClass->setObjectName(QString::fromUtf8("XlabAppMainWindowClass"));
        XlabAppMainWindowClass->resize(1349, 878);
        openDICOM = new QAction(XlabAppMainWindowClass);
        openDICOM->setObjectName(QString::fromUtf8("openDICOM"));
        openNII = new QAction(XlabAppMainWindowClass);
        openNII->setObjectName(QString::fromUtf8("openNII"));
        showCross = new QAction(XlabAppMainWindowClass);
        showCross->setObjectName(QString::fromUtf8("showCross"));
        showCross->setEnabled(false);
        showCross->setVisible(true);
        exitXlabApp = new QAction(XlabAppMainWindowClass);
        exitXlabApp->setObjectName(QString::fromUtf8("exitXlabApp"));
        exitXlabApp->setEnabled(true);
        actionc = new QAction(XlabAppMainWindowClass);
        actionc->setObjectName(QString::fromUtf8("actionc"));
        centralWidget = new QWidget(XlabAppMainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_1 = new QGridLayout();
        gridLayout_1->setSpacing(6);
        gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
        toprightvtk = new QVTKWidget(centralWidget);
        toprightvtk->setObjectName(QString::fromUtf8("toprightvtk"));

        gridLayout_1->addWidget(toprightvtk, 0, 1, 1, 1);

        bottomrightvtk = new QVTKWidget(centralWidget);
        bottomrightvtk->setObjectName(QString::fromUtf8("bottomrightvtk"));

        gridLayout_1->addWidget(bottomrightvtk, 1, 1, 1, 1);

        topleftvtk = new QVTKWidget(centralWidget);
        topleftvtk->setObjectName(QString::fromUtf8("topleftvtk"));

        gridLayout_1->addWidget(topleftvtk, 0, 0, 1, 1);

        bottomleftvtk = new QVTKWidget(centralWidget);
        bottomleftvtk->setObjectName(QString::fromUtf8("bottomleftvtk"));

        gridLayout_1->addWidget(bottomleftvtk, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout_1, 0, 1, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(230, 16777215));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        fileNameTree = new QTreeView(widget);
        fileNameTree->setObjectName(QString::fromUtf8("fileNameTree"));
        fileNameTree->setMaximumSize(QSize(211, 211));
        fileNameTree->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        verticalLayout->addWidget(fileNameTree);

        viewButton = new QPushButton(widget);
        viewButton->setObjectName(QString::fromUtf8("viewButton"));

        verticalLayout->addWidget(viewButton);

        deleteDataButton = new QPushButton(widget);
        deleteDataButton->setObjectName(QString::fromUtf8("deleteDataButton"));

        verticalLayout->addWidget(deleteDataButton);


        gridLayout->addWidget(widget, 0, 0, 1, 1, Qt::AlignTop);

        XlabAppMainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XlabAppMainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1349, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        XlabAppMainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XlabAppMainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XlabAppMainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XlabAppMainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XlabAppMainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(openDICOM);
        menu->addAction(openNII);
        menu->addSeparator();
        menu->addAction(exitXlabApp);
        menu_2->addAction(showCross);

        retranslateUi(XlabAppMainWindowClass);

        QMetaObject::connectSlotsByName(XlabAppMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *XlabAppMainWindowClass)
    {
        XlabAppMainWindowClass->setWindowTitle(QCoreApplication::translate("XlabAppMainWindowClass", "XlabAppMainWindow", nullptr));
        openDICOM->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200DICOM\346\225\260\346\215\256", nullptr));
        openNII->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200NII\346\225\260\346\215\256", nullptr));
        showCross->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200/\345\205\263\351\227\255\345\215\201\345\255\227\347\272\277", nullptr));
        showCross->setIconText(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200/\345\205\263\351\227\255\345\215\201\345\255\227\347\272\277", nullptr));
#if QT_CONFIG(tooltip)
        showCross->setToolTip(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200/\345\205\263\351\227\255\345\233\233\350\247\206\345\233\276\345\215\201\345\255\227", nullptr));
#endif // QT_CONFIG(tooltip)
        exitXlabApp->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(tooltip)
        exitXlabApp->setToolTip(QCoreApplication::translate("XlabAppMainWindowClass", "\351\200\200\345\207\272XlabApp", nullptr));
#endif // QT_CONFIG(tooltip)
        actionc->setText(QCoreApplication::translate("XlabAppMainWindowClass", "c", nullptr));
        viewButton->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\346\230\276\347\244\272", nullptr));
        deleteDataButton->setText(QCoreApplication::translate("XlabAppMainWindowClass", "\345\210\240\351\231\244\346\225\260\346\215\256", nullptr));
        menu->setTitle(QCoreApplication::translate("XlabAppMainWindowClass", "\346\211\223\345\274\200", nullptr));
        menu_2->setTitle(QCoreApplication::translate("XlabAppMainWindowClass", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XlabAppMainWindowClass: public Ui_XlabAppMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // XLABAPPMAINWINDOWNIBODP_H
